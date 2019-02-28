#include <iostream>

class A {
private:
    const int a;
    const int &b;
public:
    A() : a(0), b(a) {
        std::cout << "A()" << std::endl;
    }
    A(const A &a) : a(0), b(this->a) {
        std::cout << "A(const A &a)" << std::endl;
    }
    A &operator=(A &a) {
        std::cout << "A &operator=(A &a)" << std::endl;
        return a;
    }
};

class B {
public:
    B() {
        std::cout << "B()" << std::endl;
    }
    B(const B &b) {
        std::cout << "B(const B &b)" << std::endl;
    }
    B &operator=(B &b) {
        std::cout << "B &operator=(B &b)" << std::endl;
        return b;
    }
};

class C {
private:
    A a;
    B b;
public:
    C() {
        std::cout << "C()" << std::endl;
    }
    C(A &a, B &b) {
        this->a = a;
        this->b = b;
    }
    C(const C &c) {
        std::cout << "C(const C &c)" << std::endl;
    }
    C &operator=(C &c) {
        std::cout << "C &operator=(C &c)" << std::endl;
        return c;
    }
};

class D {
private:
    A a;
    B b;
    C c;
public:
    D() {
        std::cout << "D()" << std::endl;
    }
    D(A &a, B &b, C &c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    D(const D &d) {
        std::cout << "D(const D &d)" << std::endl;
    }
    D &operator=(D &d) {
        std::cout << "D &operator=(D &d)" << std::endl;
        return d;
    }
};

class E {
private:
    A a;
    B b;
    C c;
    D d;
public:
    E(A &a, B &b, C &c, D &d) : a(a), b(b), c(c), d(d) {}
};

int main(int argc, char *argv[]) {
    A a;
    B b;
    std::cout << "C(a, b)" << std::endl;
    C c(a, b);
    std::cout << "D(a, b, c)" << std::endl;
    D d(a, b, c);
    std::cout << "E(a, b, c, d)" << std::endl;
    E e(a, b, c, d);
    return 0;
}
