#include <boost/process.hpp>

#include <iostream>
#include <thread>

void f() {
    namespace bp = boost::process;
    bp::group g;
    bp::spawn("bash -c \"for i in {1..3}; do echo A-$i; sleep 1; done\"", g);
    bp::spawn("bash -c \"for i in {1..7}; do echo B-$i; sleep 1; done\"", g);
    for (const auto &elem : {1, 2, 3, 4, 5}) {
        std::cout << elem << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    try {
        g.wait();
    } catch (bp::process_error &error) {
        std::cout << "Caught error " << error.what() << "\n";
    }
}

int main() {
    {
        namespace bp = boost::process;
        bp::child c{"sleep 10"};
        if (!c.wait_for(std::chrono::seconds(1))) { // give it a second
            c.terminate(); // then terminate
        }
    }
    {
        namespace bp = boost::process;
        bp::group g;
        bp::child c{"sleep 10", g};
        if (!g.wait_for(std::chrono::seconds(1))) {
            g.terminate();
        }

        try {
            c.wait(); // to avoid a zombie process & get the exit code
        } catch (std::system_error &error) {
            std::cout << "Caught error " << error.what() << "\n";
        }
    }
    {
        f();
    }
}
