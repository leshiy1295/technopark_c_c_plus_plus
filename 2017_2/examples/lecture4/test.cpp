#include <iostream>

class A{
  public:
    A &operator =(const int i) {
      _b = i;
    }
    void print() {
      std::cout << _b << std::endl;
    }
  protected:
    int _b;
};

class B : public A{
  public:
    using A::operator=;
};

int main() {
  A a;
  a = 5;
  a.print();
  B b;
  b = 4;
  b.print();
  return 0;
}
