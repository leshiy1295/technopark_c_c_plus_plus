#include <iostream>

int counter = int();

struct Sample {
    std::string msg{"Abeunt studia in mores"};
    int id = ++counter;
    int n{42};

    static const std::size_t sz = sizeof n;
    Sample() {
        std::cout << "Sample()" << std::endl;
    }
    Sample(int _n) : n(_n) {
        std::cout << "Sample(int)" << std::endl;
    }
    Sample(const Sample &s) {
        std::cout << "Sample(const Sample &s)" << std::endl;
    }
    void print() {
        std::cout << id << " " << n << " " << msg << std::endl;
    }
};

struct Coord3D {
private:
    int coords[3];
public:
    Coord3D(std::initializer_list<int> l) {
        std::cout << "Coord3D(std::initializer_list<int>)" << std::endl;
        std::copy(l.begin(), l.end(), coords);
    }

    void print() {
        std::cout << coords[0] << " " << coords[1] << " " << coords[2] << std::endl;
    }
};

int main() {
    Sample s1;
    Sample s2(10);
    Sample s3 = {1};
    Sample s4{2};
    Sample s5{s4};
    s1.print();
    s2.print();
    s3.print();
    s4.print();
    s5.print();
    Coord3D a = {1, 2, 3};
    Coord3D b{4, 5, 6};
    Coord3D c{a};
    a.print();
    b.print();
    c.print();
    return 0;
}
