#include <iostream>

struct A {
    A() {
        std::cout << "A()" << std::endl;
    }
    A(A &&a) {
        std::cout << "A(A &&a)" << std::endl;
    }
};

struct B {
    B() {
        std::cout << "B()" << std::endl;
    }
    B(B &&b) {
        std::cout << "B(B &&b)" << std::endl;
    }
};

struct C {
private:
    A a;
    B b;
};

int main(int argc, char *argv[]) {
    C c1;
    C c2(std::move(c1));
    return 0;
}
