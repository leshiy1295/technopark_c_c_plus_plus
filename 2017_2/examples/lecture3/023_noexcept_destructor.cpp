struct A {
    /* BAD */
    ~A() /* noexcept */ { // try to uncomment it
        throw "Exception";
    }
};

struct B {
    /* GOOD */
    ~B() noexcept {
    }
};

int main(int argc, char *argv[]) {
    A a;
    B b;
    return 0;
}
