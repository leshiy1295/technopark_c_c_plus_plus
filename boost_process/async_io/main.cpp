#include <boost/process.hpp>

#include <iostream>
#include <vector>

int main() {
    {
        boost::asio::io_context ioc;
        std::vector<char> buf(4096);

        namespace bp = boost::process;
        bp::async_pipe ap(ioc);

        bp::child c{bp::search_path("make"), "AsyncIO", bp::std_out > ap};

        boost::asio::async_read(ap, boost::asio::buffer(buf),
            [](const boost::system::error_code &ec, std::size_t size){
                if (ec) {
                    std::cout << ec.message() << ", size=" << size << "\n";
                }
            }
        );

        ioc.run();
        c.wait();

        int result = c.exit_code();

        std::cout << "Subprocess finished with code: " << result << "\n";
        std::copy(std::begin(buf), std::end(buf), std::ostreambuf_iterator(std::cout));
    }
    {
        boost::asio::io_context ioc;
        std::vector<char> buf(4096);

        namespace bp = boost::process;
        bp::child c{bp::search_path("make"), "AsyncIO", bp::std_out > boost::asio::buffer(buf), ioc};

        ioc.run();
        c.wait();

        int result = c.exit_code();
        std::cout << "Subprocess finished with code: " << result << "\n";
        std::copy(std::begin(buf), std::end(buf), std::ostreambuf_iterator(std::cout));
    }
    {
        boost::asio::io_context ioc;
        std::future<std::string> data;

        namespace bp = boost::process;
        bp::child c{
            bp::search_path("make"), "AsyncIO", // set the input
            bp::std_in.close(),
            bp::std_out > bp::null, // so it can be written without anything
            bp::std_err > data,
            ioc
        };
        ioc.run(); // this will actually block until the compiler is finished working
        // c.wait(); // without that exit_code will show exit_code = 383 that is "still working"
        auto err = data.get();

        int result = c.exit_code();

        std::cout << "Subprocess finished with code: " << result << "\n";
        std::cout << "Errors: " << err << "\n";
    }
}
