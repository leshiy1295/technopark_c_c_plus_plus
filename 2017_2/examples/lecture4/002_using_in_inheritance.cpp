class A {
  public:
  void foo(int a) {}
  void foo(int *b) {}
};

class B : public A {
  public:
  void foo(char *s) {}
};

class C : public A {
  public:
  void foo(char *s) {}
  using A::foo;
};

int main(int argc, char *argv[]) {
  int arg = 4;
  A a;
  B b;
  C c;
  A *aa = &a;
  A *ab = &b;
  A *ac = &c;
  B *bb = &b;
  C *cc = &c;
  a.foo(arg);
  a.foo(&arg);
  b.foo(arg);
  b.foo(&arg);
  b.foo("asd");
  c.foo("asd");
  c.foo(arg);
  c.foo(&arg);
  aa->foo("asd");
  aa->foo(arg);
  aa->foo(&arg);
  ab->foo("asd");
  ab->foo(arg);
  ab->foo(&arg);
  ac->foo("asd");
  ac->foo(arg);
  ac->foo(&arg);
  bb->foo("asd");
  bb->foo(arg);
  bb->foo(&arg);
  cc->foo("asd");
  cc->foo(arg);
  cc->foo(&arg);
  return 0;
}
