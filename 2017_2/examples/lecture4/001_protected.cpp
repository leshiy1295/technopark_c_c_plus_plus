class A {
  private:
    int a;
    void foo() {}
  protected:
    int b;
    void bar() {}
  public:
    int c;
    void baz() {}
};

class B : protected A {
  void quux() {
    a = 1;
    foo();
    b = 2;
    bar();
    c = 3;
    baz();
  }
};

int main(int argc, char *argv[]) {
  /*A a;
  a.a = 1;
  a.foo();
  a.b = 2;
  a.bar();
  a.c = 3;
  a.baz();*/
  B b;
  b.a = 1;
  b.foo();
  b.b = 2;
  b.bar();
  b.c = 3;
  b.baz();
  return 0;
}
