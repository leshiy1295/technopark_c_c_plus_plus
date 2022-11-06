#include <boost/process.hpp>
#include <boost/process/extend.hpp>

#include <iostream>

namespace {
    namespace bp = boost::process;
}

namespace {
    struct HelloHandler : bp::extend::handler {
        template <typename Executor>
        void on_success(Executor &exec) const {
            std::cout << "on_success\n";
        }
    };
}

namespace {
    struct AsyncHandler : bp::extend::handler, bp::extend::require_io_context {
        template <typename Executor>
        void on_setup(Executor &exec) {
            boost::asio::io_context &ios = bp::extend::get_io_context(exec.seq);
            std::cout << "Hello from async::on_setup handler\n";
        }
    };
}

namespace {
    struct AsyncExitHandler : bp::extend::async_handler {
        template <typename Executor>
        std::function<void(int, const std::error_code &)> on_exit_handler(Executor &exec) {
            return [](int exit_code, const std::error_code &error_code) {
                std::cout << "Hello from exit handler\n";
            };
        }
    };
}

namespace {
    struct ExecutorDependantHelloHandler : bp::extend::handler {
        template <typename Sequence>
        void on_success(bp::extend::posix_executor<Sequence> &exec) {
            std::cout << "posix-exe: " << exec.exe << "\n";
        }

        template <typename Sequence>
        void on_success(bp::extend::windows_executor<char, Sequence> &exec) {
            // exec.exe might be a nullptr on windows
            if (exec.exe != nullptr) {
                std::cout << "windows-exe: " << exec.exe << "\n";
            } else {
                std::cout << "windows didn't use exe\n";
            }
        }

        template <typename Sequence>
        void on_success(bp::extend::windows_executor<wchar_t, Sequence> &exec) {
            // exec.exe might be a nullptr on windows
            if (exec.exe != nullptr) {
                std::cout << L"windows-exe: " << exec.exe << "\n";
            } else {
                std::cout << "windows didn't use exe\n";
            }
        }
    };
}

int main() {
    {
        std::cout << boost::this_process::get_id() << "\n";

        bp::child c{"ls", bp::extend::on_setup([](auto &executor){
            std::cout << boost::this_process::get_id() << "\n";
        })};
        c.wait();
    }
    {
        bp::child c{"pwd", HelloHandler{}};
        c.wait();
    }
    {
        boost::asio::io_context ioc;
        bp::child c{"pwd", AsyncHandler{}, ioc};
        ioc.run();
    }
    {
        boost::asio::io_context ioc;
        bp::child c{"pwd", AsyncExitHandler{}, ioc};
        ioc.run();
    }
    {
        auto on_setup = [](auto &exec) { // always fail handler
            std::error_code ec{42, std::system_category()};
            exec.set_error(ec, "some error message");
        };
        try {
            bp::child c{"ls -la", bp::extend::on_setup=on_setup};
            c.wait();
        } catch (bp::process_error &error) {
            std::cout << "Caught " << error.what() << "\n";
        }
    }
    {
        bp::child c{"pwd", ExecutorDependantHelloHandler{}};
        c.wait();
    }
}
