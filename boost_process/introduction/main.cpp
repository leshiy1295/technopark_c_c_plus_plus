#include <boost/process.hpp>

#include <iostream>
#include <string>

using namespace boost::process;

int main() {
    ipstream pipe_stream;
    child c("gcc --version", std_out > pipe_stream);

    std::string line;

    while (pipe_stream && std::getline(pipe_stream, line) && !line.empty()) {
        std::cerr << line << "\n";
    }

    c.wait();
}
