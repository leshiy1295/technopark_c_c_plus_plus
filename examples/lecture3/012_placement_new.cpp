#include <iostream>

class A {
private:
    int counter;
public:
    A() {
        std::cout << "A()" << std::endl;
    }
    A(int counter) : counter(counter) {
        std::cout << "A(int)" << std::endl;
    }
    void inc() {
        ++counter;
    }
    void print() {
        std::cout << counter << std::endl;
    }
    ~A() {
        std::cout << "~A()" << std::endl;
    }
};

int main(int argc, char *argv[]) {
    char *buffer = new char[sizeof(A)];
    A *a1 = new (buffer) A(0);
    a1->print();
    a1->inc();
    a1->inc();
    a1->print();
    a1->~A();
    A *a2 = new (buffer) A();
    a2->print();
    a2->inc();
    a2->inc();
    a2->print();
    a2->~A();
    delete []buffer;
    return 0;
}
