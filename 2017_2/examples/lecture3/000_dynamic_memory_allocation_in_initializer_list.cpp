class A {
  int _N;
  int *_a;
  public:
  A(int N): _a(new int[_N]), _N(N) {}
  ~A() { delete[] _a;} 
};

int main() {
  A a(10);
  return 0;
}
