#include <boost/process.hpp>

#include <iostream>

int main() {
    {
        namespace bp = boost::process;
        // get a handle to the current environment
        auto env = boost::this_process::environment();
        // add a variable to the current environment
        env["VALUE_1"] = "foo";

        for (const auto &elem : env) {
            std::cout << elem.get_name() << " -> " << elem.to_string() << "\n";
        }

        // copy it into an environment separate to the one of this process
        bp::environment env_ = env;
        // append two values to a variable in the new env
        for (const auto &value : {"bar1", "bar2"}) {
            env_["VALUE_2"] += value;
        }

        // launch a process with `env_`
        bp::system("env", env_);
    }
    {
        namespace bp = boost::process;
        bp::system("env", bp::env["VALUE_1"]="foo", bp::env["VALUE_2"]+="bar1");
    }
}
