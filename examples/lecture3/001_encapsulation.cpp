#include <iostream>

class A {
    int a;
private:
    int b;
public:
    int c;
    void print() {
        std::cout << "A: " << a << " " << b << " " << c << std::endl;
    }
};

struct B {
    int a;
private:
    int b;
public:
    int c;
    void print() {
        std::cout << "B: " << a << " " << b << " " << c << std::endl;
    }
};

union C {
    int a;
private:
    int b;
public:
    int c;
    void print() {
        std::cout << "C: " << a << " " << b << " " << c << std::endl;
    }
};

int main(int argc, char *argv[]) {
    A a;
    a.print();
    /*a.a = 1;
    a.print();*/
    /*a.b = 2;
    a.print();*/
    a.c = 3;
    a.print();
    B b;
    b.print();
    b.a = 1;
    b.print();
    /*b.b = 2;
    b.print();*/
    b.c = 3;
    b.print();
    C c;
    c.print();
    c.a = 1;
    c.print();
    /*c.b = 2;
    c.print();*/
    c.c = 3;
    c.print();
    return 0;
}
