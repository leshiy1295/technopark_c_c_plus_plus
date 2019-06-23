#include <iostream>
#include <string>
#include <vector>

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
  std::vector<A> my_magic_vector;
  A some_a("test", 1);
  my_magic_vector.reserve(10);

  std::cout << "\nemplace_back:\n";
  my_magic_vector.emplace_back("String1", 1000);
  std::cout << "----\n";
  my_magic_vector.emplace_back(A("String1", 1000));

  std::cout << "\npush_back:\n";
  my_magic_vector.push_back(A("String2", 2000));
  //my_magic_vector2.push_back("String2", 2000); error

  std::cout << "\nappending existing object:\n";
  my_magic_vector.emplace_back(some_a);
  my_magic_vector.push_back(some_a);

  return 0;
}
