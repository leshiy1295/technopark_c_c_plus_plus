#include <iostream>

namespace N1 {
    void foo() {
        std::cout << "N1::foo()" << std::endl;
    }
    void bar() {
        std::cout << "N1::bar()" << std::endl;
    }
}

namespace N2 {
    void foo() {
        std::cout << "N2::foo()" << std::endl;
    }
    void baz() {
        std::cout << "N2::baz()" << std::endl;
    }
}

int main() {
    using namespace N1;
    //using namespace N2;
    using N2::baz;
    foo();
    bar();
    baz();
    return 0;
}
