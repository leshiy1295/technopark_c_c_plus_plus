#include <iosfwd>
#include <iostream>
#include <iterator>
#include <queue>
#include <stack>
#include <string>
#include <type_traits>
#include <vector>
#include <set>

using namespace std;

int main() {
  vector<int> e = {1, 2, 3, 4, 5, 6};
  auto pred = [&](int el) -> bool { return el < 5; };
  e.erase(std::remove_if(e.begin(), e.end(), pred), e.end());
//  e.erase(std::remove_if(e.begin(), e.end(), std::bind2nd(std::less<int>(), 5)),
//            e.end());
  for (int el : e) {
    std::cout << el << " ";
  }
  std::cout << endl;
  return 0;
}
