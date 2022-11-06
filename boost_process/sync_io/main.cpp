#include <boost/process.hpp>

#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> read_outline(const std::string &file) {
    namespace bp = boost::process;
    bp::ipstream is; // reading pipe-stream

    bp::child c{bp::search_path("nm"), file, bp::std_out > is};

    std::vector<std::string> data;
    std::string line;

    while (c.running() && std::getline(is, line) && !line.empty()) {
        data.push_back(line);
    }

    c.wait();

    return data;
}

namespace {
    struct Line : std::string {
        friend std::istream &operator>>(std::istream &is, Line &line) {
            return std::getline(is, line);
        }
    };
}

std::vector<std::string> read_demangled_outline(const std::string &file) {
    namespace bp = boost::process;
    bp::pipe p;
    bp::ipstream is;

    std::vector<std::string> outline;

    // we just use the same pipe, so the output of nm is directly passed as input to c++filt
    bp::child nm{bp::search_path("nm"), file, bp::std_out > p};
    bp::child filt{bp::search_path("c++filt"), bp::std_in < p, bp::std_out > is};

    // when nm finished the pipe closes and c++filt exits
    std::copy(std::istream_iterator<Line>(is), std::istream_iterator<Line>(), std::back_inserter(outline));

    nm.wait();
    filt.wait();

    return outline;
}

int main() {
    {
        for (const auto &line : read_outline("SyncIO")) {
            std::cout << line << "\n";
        }
    }
    {
        namespace bp = boost::process;
        bp::opstream in;
        bp::ipstream out;

        bp::child c("c++filt", bp::std_out > out, bp::std_in < in);

        std::string mangled_value = "_ZN5boost7process8tutorialE";

        in << mangled_value << std::endl;
        std::string demangled_value;
        out >> demangled_value;

        c.terminate();

        std::cout << "Demangled<" << mangled_value << "> -> " << demangled_value << "\n";
    }
    {
        auto outline = read_demangled_outline("SyncIO");
        std::copy(std::begin(outline), std::end(outline), std::ostream_iterator<std::string>(std::cout, "\n"));
    }
}
