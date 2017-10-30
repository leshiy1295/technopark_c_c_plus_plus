#include <iostream>
struct A {
  virtual ~A() = 0;
};

A::~A() {std::cout << "~A()\n";}

struct B : public A {
  ~B() {std::cout << "~B()\n";}
};

int main(int argc, char *argv[]) {
  //A a;
  B b;
  A *ab = new B;
  delete ab;
  return 0;
}
