#include <iostream>
#include <typeinfo>


namespace RTTI {
class A {
 public:
  A() { std::cout << "ctor A()" << std::endl; }

  virtual void foo() { std::cout << std::endl << std::endl; }

  int a = 0;
  int b = 0;
};

class B : public A {
 public:
  B() { std::cout << "ctor A()" << std::endl; }
};

class C {
 public:
  C() = default;
  virtual void foo() {}
};


void foo(A &a) {
  try {
    B &beta = dynamic_cast<B&>(a);
  }
  catch(std::bad_cast) {
    std::cout << "bad cast";
  }
}

}  // namespace diamond_problem



int main() {
  using namespace RTTI;
  {
    A *a = new B;
    if (B *b = dynamic_cast<B *>(a)) {
      // успешно…
      std::cout << "b is a child of A";
    } else {
      std::cout << "b is not a child of A";
    }
  }
  std::cout << std::endl;
  {
    C *c = new C;
    if (B *b = dynamic_cast<B *>(c)) {
      // успешно…
      std::cout << "b is a child of C";
    } else {
      std::cout << "b is not a child of C";
    }
  }
  std::cout << std::endl;
  {
    B a;
    foo(a);
  }
  std::cout << std::endl;
  {
    A *a = new A;
    A *b = new B;
    if(typeid(a) == typeid(A*)) {
      std::cout << "a is A*" << std::endl;
    }

    if(typeid(*a) == typeid(A)) {
      std::cout << "*a is A" << std::endl;
    }

    if(typeid(b) == typeid(B*)) {
      std::cout << "b is B*" << std::endl;
    }
  }

  {
    struct Base {}; // non-polymorphic
    struct Derived : Base {};

    struct Base2 { virtual void foo() {} }; // polymorphic
    struct Derived2 : Base2 {};

    Derived d1;
    Base& b1 = d1;
    std::cout << "reference to non-polymorphic base: " << typeid(b1).name() << '\n';

    Derived2 d2;
    Base2& b2 = d2;
    std::cout << "reference to polymorphic base: " << typeid(b2).name() << '\n';
  }

  return 0;
}
