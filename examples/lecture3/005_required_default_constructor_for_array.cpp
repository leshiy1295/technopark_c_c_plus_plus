#include <iostream>

struct Sample {
    //Sample() {}
    Sample(int a/* = 0*/): a(a) {}

    void print() {
        std::cout << "print()" << std::endl;
    }

private:
    int a;
};

int main(int argc, char *argv[]) {
    const int N = 3;
    Sample samples[N];
    for (int i = 0; i < N; ++i) {
        samples1[i].print();
    }
    Sample *samples1 = new Sample[N];
    for (int i = 0; i < N; ++i) {
        samples1[i].print();
    }
    delete []samples1;
    return 0;
}
