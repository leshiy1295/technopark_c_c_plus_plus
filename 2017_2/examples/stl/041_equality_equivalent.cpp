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

struct A {
public:
  A(const std::string &s) : tmp(s){};

  bool operator<(const A &rhs) const {
    std::cout << "< called this " << tmp << " rhs = " << rhs.tmp << endl;
    return tmp < rhs.tmp;
  }

  bool operator==(const A &rhs) const {
    std::cout << " == called this " << tmp << " rhs = " << rhs.tmp << endl;
    return tmp == rhs.tmp;
  }
  std::string tmp;
};

int main() {
  std::set<A> s;
  s.insert(A("1")); // empty


  s.insert(A("2"));
  // s.insert(A("3"));

  std::cout << "---\n";
  auto it = s.find(A("2"));
  std::cout << "---\n";
  if (it != s.end())
    std::cout << "2 has found\n";

  it = std::find(s.begin(), s.end(), A("2"));
  if (it != s.end())
    std::cout << "2 has found\n";

  return 0;
}
