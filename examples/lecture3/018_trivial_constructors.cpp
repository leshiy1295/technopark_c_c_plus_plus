#include <iostream>

struct A {
    int a;
    void print() {
        std::cout << "A::print(" << a << ")" << std::endl;
    }
};

struct B {
    int b;
    void print() {
        std::cout << "B::print(" << b << ")" << std::endl;
    }
};

struct C {
    A a;
    B b;
    void print() {
        a.print();
        b.print();
    }
};

int main(int argc, char *argv[]) {
    char buffer[sizeof(C)];
    reinterpret_cast<C *>(buffer)->print();
    return 0;
}
