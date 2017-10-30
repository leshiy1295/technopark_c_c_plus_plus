#include <iostream>

struct Beta {
    Beta(std::string s) : msg(s) {
        std::cout << "Beta(std::string)" << std::endl;
    }
    Beta() {
        std::cout << "Beta()" << std::endl;
    }
    Beta(const Beta &b) {
        std::cout << "Beta(const Beta&)" << std::endl;
    }
    Beta(Beta &&b) {
        std::cout << "Beta(Beta &&b)" << std::endl;
    }
    /* Try only this implementation instead of two next
    Beta &operator=(Beta rhs) {
        std::cout << "operator=(Beta)" << std::endl;
        msg = std::move(rhs.msg);
        return *this;
    }*/
    Beta &operator=(Beta const &b) {
        std::cout << "operator=(Beta const &)" << std::endl;
        this->msg = b.msg;
        return *this;
    }
    Beta &operator=(Beta &&rhs) {
        std::cout << "operator=(Beta &&)" << std::endl;
        msg = std::move(rhs.msg);
        return *this;
    }
    std::string msg;
};

int main(int argc, char *argv[]) {
    Beta beta{"Per aspera ad astra"}, gamma;
    gamma = beta;
    gamma = std::move(beta);
    std::cout << gamma.msg << std::endl
              << beta.msg << std::endl;
    return 0;
}
