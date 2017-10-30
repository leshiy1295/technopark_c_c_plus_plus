#include <iostream>
using namespace std;

class A {
  public:
    void foo(char sym = 'A') {
      std::cout << "A::FOO(" << sym << ")" << std::endl;
    }
};

class B : public A {
  public:
    virtual void foo(char sym = 'B') {
      std::cout << "B::FOO(" << sym << ")" << std::endl;
    }
};

class C : public B {
  public:
    void foo(char sym = 'C') {
      std::cout << "C::FOO(" << sym << ")" << std::endl;
    }
};

int main(int argc, char *argv[]) {
  A a;
  B b;
  C c;
  A *aa = &a;
  A *ab = &b;
  A *ac = &c;
  B *bb = &b;
  B *bc = &c;
  C *cc = &c;
  a.foo();
  b.foo();
  c.foo();
  aa->foo();
  ab->foo();
  ac->foo();
  ab->A::foo();
  bb->A::foo();
  bb->B::foo();
  bb->foo();
  bc->foo();
  cc->foo();
  cc->A::foo();
  cc->B::foo();
  return 0;
}
