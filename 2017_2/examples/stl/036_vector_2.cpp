#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

struct A {
  std::string param1;
  int param2;

  A(std::string p_name, int p_year)
      : param1(std::move(p_name)), param2(p_year) {
    std::cout << "I am being constructed.\n";
  }

  A(A &&other) : param1(std::move(other.param1)), param2(other.param2) {
    std::cout << "I am being moved.\n";
  }

  A(const A &other) : param1(other.param1), param2(other.param2) {
    std::cout << "I am being copied.\n";
  }

  A &operator=(const A &other) = default;
};

// --------------------------------
int main() {
  A some_a("test", 1);
  std::vector<A> v;

  v.reserve(2);
  assert(v.capacity() == 2);

  v[0] = some_a;
  v[1] = some_a;

  for(decltype(v)::iterator it = v.begin(); it < v.end(); it++) {
    std::cout << (*it).param1;
  }

  std::cout << v[0].param1;
  v.reserve(100);
  assert(v.capacity() == 100);
  std::cout << v[0].param1;

  v[2] = some_a;
  v[3] = some_a;

  for(auto it = v.begin(); it < v.end(); it++) {
    std::cout << (*it).param1;
  }

  return 0;
}
