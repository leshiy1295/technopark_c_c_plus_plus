#include <iostream>

struct A {
    /* BAD */
    ~A()  noexcept  { // try to uncomment it
        throw "Exception";
    }
};

struct B {
    /* GOOD */
    ~B() noexcept {
    }
};

int main(int argc, char *argv[]) {
    try {
        A a;
    } catch(...) {
        std::cout << "catched";
    }

    B b;
    return 0;
}
