#include <iostream>

struct A {
private:
    int ipr;
    double dpr;
public:
    A() = delete;
    A(int ipr, double dpr) : ipr(ipr), dpr(dpr) {}
    A(A &&) = default;
    A(A &) = default;
    A &operator=(A const &) = default;
};

int main() {
    A //a1,
      a2(1, 4.2),
      a3(std::move(a2)),
      a4(a2);
    a4 = a3;
    return 0;
}
