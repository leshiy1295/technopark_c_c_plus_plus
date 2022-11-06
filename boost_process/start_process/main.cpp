#include <boost/process.hpp>

#include <iostream>

int main() {
    {
        int result = std::system("make StartProcess");
        std::cout << result << "\n";
    }
    {
        namespace bp = boost::process;
        int result = bp::system("make StartProcess");
        std::cout << result << "\n";
    }
    {
        namespace bp = boost::process;
        int result = bp::system("/usr/bin/make", "StartProcess");
        std::cout << result << "\n";
    }
    {
        namespace bp = boost::process;
        bp::filesystem::path p = "/usr/bin/make";
        int result = bp::system(p, "StartProcess");
    }
    {
        namespace bp = boost::process;
        auto p = bp::search_path("make");
        bp::system(p, "StartProcess");
    }
    {
        namespace bp = boost::process;
        bp::spawn(bp::search_path("ls"), "-la");
    }
    {
        namespace bp = boost::process;
        bp::child c{bp::search_path("make"), "StartProcess"};
        std::uint64_t running_counter = 0;
        while (c.running()) {
            running_counter += 1;
        }
        c.wait();
        std::cout << "Running counter = " << running_counter << "\n";
        int result = c.exit_code();
        std::cout << result << "\n";
    }
    {
        namespace bp = boost::process;
        std::error_code error_code;
        bp::system("qwertyasdfghzxcvb", error_code);
        if (error_code) {
            std::cout << "Error " << error_code << ": " << error_code.message() << "\n";
        } else {
            std::cout << "No error\n";
        }
    }
    {
        namespace bp = boost::process;
        bp::system("make StartProcess", bp::std_out > stdout, bp::std_err > stderr, bp::std_in < stdin);
        bp::system("make StartProcess", bp::std_out > bp::null);
        bp::system("make StartProcess", bp::std_out > "gcc_out.log");
    }
}
