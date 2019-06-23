#include <iostream>

namespace diamond_problem {
class A {
 public:
  A() { std::cout << "ctor A()" << std::endl; }
  explicit A(int a) { std::cout << "ctor A() << a = " << a << std::endl; }

  virtual void foo() { std::cout << std::endl << std::endl; }

  int a = 0;
  int b = 0;
};

class B :  public virtual A {
 public:
  B() { std::cout << "ctor B()" << std::endl; }
  explicit B(int a) : A(a) { std::cout << "ctor B() << a " << a << std::endl; }
};

class C :  public virtual A {
 public:
  C() { std::cout << "ctor C()" << std::endl; }
};

class NonVirtual {
 public:
  NonVirtual() { std::cout << "ctor Nonvirtual()" << std::endl; }
};

class D : public NonVirtual, public B, public C  {
 public:
  D() : A(5), B(5) { std::cout << "ctor D()" << std::endl; }
};
} // namespace diamond_problem

namespace another_inh_problem {
class B {
 public:
  B() { std::cout << "ctor B()" << std::endl; }

  virtual void foo() { std::cout << std::endl << std::endl; }
};

class C {
 public:
  C() { std::cout << "ctor C()" << std::endl; }

  virtual void foo() { std::cout << std::endl << std::endl; }
};

class D : public B, public C {
 public:
  using B::foo;
  D() { std::cout << "ctor D()" << std::endl; }
};
} // namespace another_inh_problem

int main() {
  {
    using namespace diamond_problem;
    D d1;
    // d1.foo();

//    std::cout << sizeof(diamond_problem::A) << std::endl;
//    std::cout << sizeof(diamond_problem::B) << std::endl;
//    std::cout << sizeof(diamond_problem::C) << std::endl;
//    std::cout << sizeof(diamond_problem::D) << std::endl;
//    std::cout << std::endl;
  }
  std::cout << std::endl;
  {
    another_inh_problem::D d;
    d.foo();
  }
  return 0;
}
