#include <boost/asio/steady_timer.hpp>
#include <boost/asio/readable_pipe.hpp>
#include <boost/asio/read_until.hpp>

#include <boost/process.hpp>
#include <boost/process/v2.hpp>

#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        bp::v2::process proc(ctx, bp::search_path("cp"), { "Makefile", "Makefile-copy" });
        assert(proc.wait() == 0);
    }
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        bp::v2::process proc(ctx, bp::search_path("ls"), {});
        assert(proc.wait() == 0);
        assert(proc.native_exit_code() == 0);
    }
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        bp::v2::process proc(ctx, bp::search_path("bash"), {"-c", "for i in {1..10}; do echo $i; sleep 1; done"});
        std::this_thread::sleep_for(std::chrono::seconds(5));
        proc.terminate();
        assert(proc.wait() == SIGKILL);
    }
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        bp::v2::process proc(ctx, bp::search_path("bash"), {"-c", "for i in {1..10}; do echo $i; sleep 1; done"});
        std::this_thread::sleep_for(std::chrono::seconds(5));
        proc.request_exit();
        assert(proc.wait() == SIGTERM);
    }
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        bp::v2::process proc(ctx, bp::search_path("bash"), {"-c", "for i in {1..10}; do echo $i; sleep 1; done"});
        std::this_thread::sleep_for(std::chrono::seconds(5));
        proc.interrupt();
        assert(proc.wait() == 0);
    }
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        assert(bp::v2::execute(bp::v2::process{ctx, bp::search_path("ls"), {}}) == 0);
    }
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        boost::asio::steady_timer timeout{ctx, std::chrono::seconds(1)};

        boost::asio::cancellation_signal sig;
        bp::v2::async_execute(
            bp::v2::process(ctx, bp::search_path("bash"), {"-c", "for i in {1..10}; do echo $i; sleep 1; done"}),
            boost::asio::bind_cancellation_slot(sig.slot(), [&](boost::system::error_code error_code, int exit_code) {
                std::cout << "cancelling timeout, exit_code=" << exit_code << "\n";
                timeout.cancel(); // we're done
            })
        );

        timeout.async_wait([&](boost::system::error_code error_code) {
            if (error_code) { // we were cancelled, do nothing
                return;
            }
            std::cout << "soft timeout - sending request_exit()\n";
            sig.emit(boost::asio::cancellation_type::partial);
            // request exit first, but terminate after another sec
            timeout.expires_after(std::chrono::seconds(1));
            timeout.async_wait([&](boost::system::error_code error_code){
                if (!error_code) {
                    sig.emit(boost::asio::cancellation_type::terminal);
                }
            });
        });
        ctx.run();
    }
    {
        namespace bp = boost::process;
        auto ctx = boost::asio::io_context{};
        bp::v2::execute(bp::v2::process{ctx, "/bin/ls", { "-la" }, bp::v2::process_start_dir{"/home"}});
    }
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        boost::asio::readable_pipe rp{ctx};

        bp::v2::process proc{ctx, bp::search_path("make"), { "V2" }, bp::v2::process_stdio{ {/* default for stdin */}, rp, stderr }};
        std::vector<char> buffer(4096);

        boost::system::error_code ec;
        rp.read_some(boost::asio::buffer(buffer), ec);
        if (ec) {
            std::cerr << "Error occured " << ec.message() << "\n";
        }
        std::copy(std::begin(buffer), std::end(buffer), std::ostreambuf_iterator(std::cout));
        proc.wait();
    }
    {
        namespace bp = boost::process;
        boost::asio::io_context ctx;
        bp::v2::popen proc(ctx, bp::search_path("c++filt"), {});
        boost::asio::write(proc, boost::asio::buffer("_ZN5boost7process8tutorialE\n"));
        std::string line;
        boost::asio::read_until(proc, boost::asio::dynamic_buffer(line), '\n');
        std::cout << "Received " << line << "\n";
    }
}
