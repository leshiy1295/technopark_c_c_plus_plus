#include <iosfwd>
#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

template <typename Derived> struct Base {
  void interface() { static_cast<Derived *>(this)->implementation(); }
  void implementation() { std::cout << "Implementation Base" << std::endl; }
};

struct Derived1 : Base<Derived1> {
  void implementation() { std::cout << "Implementation Derived1" << std::endl; }
};

struct Derived2 : Base<Derived2> {
  void implementation() { std::cout << "Implementation Derived2" << std::endl; }
};

struct Derived3 : Base<Derived3> {};

template <typename T> void execute(Base<T> &base) { base.interface(); }

int main() {
  Derived1 d1;
  execute(d1);

  Derived2 d2;
  execute(d2);

  Derived3 d3;
  execute(d3);

  int i = INT_MAX + 100;
  std::cout << std::endl;
  return 0;
}
