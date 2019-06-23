#include <iosfwd>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

// --------------------------------
class Date {
public:
  Date() = default;
  explicit Date(const std::string &str){};
  Date(const Date &rhs){};

  Date &operator=(const std::string &rhs) { return *this; }
  bool operator==(const std::string &rhs) { return true; }

  friend ostream &operator<<(ostream &out, const Date &c) {
    cout << "Date\n";
    return out;
  };

  friend istream &operator>>(istream &in, Date &c) {
    cout << "Date: \n";
    in >> c.date;
    return in;
  }

  static Date TodaysDate() { return Date("29/04/19"); }
  string date;
};

int main() {
  // std::binary_function
  std::stack<int> s; // deque as default
  std::queue<int> q; // deque as default
  std::priority_queue<int> pq; // vector as default

  // std::find_if
  vector<Date> e;
  copy(istream_iterator<Date>(cin), istream_iterator<Date>(), back_inserter(e));
  vector<Date>::iterator first = find(e.begin(), e.end(), "01/01/95");
  vector<Date>::iterator last = find(e.begin(), e.end(), "12/31/95");
  *last = "12/30/95";
  copy(first, last, ostream_iterator<Date>(cout, "\n"));
  e.insert(--e.end(), Date::TodaysDate());
  copy(first, last, ostream_iterator<Date>(cout, "\n"));
  return 0;
}
