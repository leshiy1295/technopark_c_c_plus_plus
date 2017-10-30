#include <iostream>

struct Sample {
    Sample(int ipr = 0, double dpr = 0.0): ipr(ipr), dpr(dpr) {}
    void print() {
        std::cout << ipr << " " << dpr << " " << std::endl;
    }
private:
    int ipr;
    double dpr;
};

int main(int argc, char *argv[]) {
    Sample s;
    s.print();
    Sample s1(10);
    s1.print();
    Sample s2(10, 5);
    s2.print();
    return 0;
}
