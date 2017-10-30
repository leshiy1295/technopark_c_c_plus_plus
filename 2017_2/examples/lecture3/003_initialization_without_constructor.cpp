#include <iostream>
#include <string>

struct Sample {
    int int_prm;
    double dbl_prm;
    std::string std_prm;
    void print() {
        std::cout << int_prm << " " << dbl_prm << " " << std_prm << std::endl;
    }
};

int main(int argc, char *argv[]) {
    Sample sample = {1, -3.14, "dictum factum"};
    sample.print();
    Sample cpp11sample{1, -3.14, "dictum factum"}; // c++11
    sample.print();
    return 0;
}
