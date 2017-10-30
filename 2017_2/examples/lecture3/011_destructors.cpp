#include <iostream>

class A {
public:
    A() {
        std::cout << "A()" << std::endl;
    }
    A(const A &a) {
        std::cout << "A(const &a)" << std::endl;
    }
    A &operator=(A &a) {
        std::cout << "A &operator=(A &a)" << std::endl;
        return a;
    }
    ~A() {
        std::cout << "~A()" << std::endl;
    }
};

class B {
public:
    B() {
        std::cout << "B()" << std::endl;
    }
    B(const B &b) {
        std::cout << "B(const &b)" << std::endl;
    }
    B &operator=(B &b) {
        std::cout << "B &operator=(B &b)" << std::endl;
        return b;
    }
    ~B() {
        std::cout << "~B()" << std::endl;
    }
};

class C {
private:
    A a;
    B b;
public:
    C(A &a, B &b) : a(a), b(b) {}
    ~C() {
        std::cout << "~C()" << std::endl;
    }
};

int main(int argc, char *argv[]) {
    A a;
    B b;
    std::cout << "C(a, b)" << std::endl;
    C c(a, b);
    C *pc;
    {
        std::cout << "C(a, b)" << std::endl;
        C c(a, b);
        std::cout << "C(a, b)" << std::endl;
        pc = new C(a, b);
        std::cout << "end of scope" << std::endl;
    }
    std::cout << "destructor call" << std::endl;
    delete pc;
    std::cout << "end of scope" << std::endl;
    return 0;
}
