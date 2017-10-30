#include <iostream>

struct A {
  A() {std::cout << "A::A()\n";}
  void foo() {std::cout << "A::foo()\n";}
  void foo(int a) {std::cout << "A::foo(" << a << ")\n";}
  ~A() {std::cout << "A::~A()\n";}
};

struct B {
  B() {std::cout << "B::B()\n";}
  void bar() {std::cout << "B::bar()\n";}
  void foo() {std::cout << "B::foo()\n";}
  void foo(char *s) {std::cout << "B::foo(" << s << ")\n";}
  ~B() {std::cout << "B:~B()\n";}
};

struct C : public B, public A {
  C() {std::cout << "C::C()\n";}
  void baz() {
    A::foo();
    B::bar();
  }
  ~C() {std::cout << "C::~C()\n";}
};

int main(int argc, char *argv[]) {
  std::cout << "A a\n";
  A a;
  std::cout << "B b\n";
  B b;
  std::cout << "C c\n";
  C c;
  a.foo();
  a.foo(5);
  b.bar();
  b.foo();
  b.foo("ASD");
  c.A::foo();
  c.B::bar();
  c.baz();
  return 0;
}
