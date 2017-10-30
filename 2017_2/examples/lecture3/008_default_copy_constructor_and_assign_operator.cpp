#include <iostream>
#include <string.h>

class A {
private:
    char *s;
public:
    A(const char *s) : s(new char[strlen(s)]) {
        std::cout << "A()" << std::endl;
        strcpy(this->s, s);
    }
    /*A(const A &a) : s(new char[strlen(a.s)]) {
        std::cout << "A(const A &a)" << std::endl;
        strcpy(this->s, a.s);
    }*/
    A(const A &a) : A(a.s) { // c++11
        std::cout << "A(const &a)" << std::endl;
    }
    A &operator=(A const &a) {
        std::cout << "A &operator=(A &a)" << std::endl;
        A tmp(a);
        std::swap(this->s, tmp.s);
        return *this;
    }
    ~A() {
        std::cout << "~A()" << std::endl;
        delete []s;
    }
    void print() {
        std::cout << s << std::endl;
    }
};

class B {
private:
    char *s;
public:
    B(const char *s) : s(new char[strlen(s)]) {
        std::cout << "B()" << std::endl;
        strcpy(this->s, s);
    }
    ~B() {
        std::cout << "~B()" << std::endl;
        delete []s;
    }
    void print() {
        std::cout << s << std::endl;
    }
private:
    /*B(const B &b) {
        std::cout << "B(const B &b)" << std::endl;
    }*/
    B(const B &b);
    B &operator=(B &b);
    friend class C;
};

class C {
private:
    B b;
public:
    C(B &b) : b(b) {}
};

int main(int argc, char *argv[]) {
    char s[] = "Hello";
    A a1(s);
    a1.print();
    A a2(a1);
    a2.print();
    a2 = a1;
    a2.print();
    B b1(s);
    b1.print();
    /*B b2(b1);
    b2.print();
    b2 = b1;
    b2.print();*/
    /*B *b2 = &b1;
    b2->print();
    *b2 = b1;
    b2->print();*/
    C c(b1);
    return 0;
}
