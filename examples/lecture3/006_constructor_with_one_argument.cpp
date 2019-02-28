#include <iostream>

struct Sample {
    Sample(int prm = 0, double drm = 0): _prm(prm), _drm(drm) {}
    void print() {
        std::cout << _prm << std::endl;
    }
private:
    int _prm;
    double _drm;
};

int main(int argc, char *argv[]) {
    Sample sample1(10),
           sample2 = Sample(10),
           sample3 = 10;
    sample1.print();
    sample2.print();
    sample3.print();
    /*Sample sample4{10}; // c++11
    sample4.print();*/
    Sample array1[] = {10, -5, 0, 127};
    std::cout << "array1[]" << std::endl;
    for (size_t i = 0; i < sizeof(array1) / sizeof(*array1); ++i) {
        array1[i].print();
    }
    const int N = 5;
    Sample array2[N] = {
        Sample(10, 0.1),
        Sample(-5, -3.6),
        Sample(0, 0.0),
        Sample() // default constructor
    };
    std::cout << "array2[" << N << "]" << std::endl;
    for (int i = 0; i < N; ++i) {
        array2[i].print();
    }
    return 0;
}
