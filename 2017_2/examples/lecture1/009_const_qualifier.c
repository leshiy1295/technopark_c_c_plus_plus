void foo(int a, const int b, int const c, int *d, const int *e, int * const f, const int * const g) {
    a = 10;
    //b = 20;
    //c = 30;
    *d = a;
    d = &a;
    //*e = b;
    e = &b;
    *f = c;
    //f = &c;
    //*g = a;
    //g = &a;

    // compatibility
    a = b;
    //b = a;
    //d = e;
    d = f;
    //d = g;
    d = &a;
    e = d;
    e = f;
    e = g;
    e = &a;
    //f = d;
    //f = e;
    //f = g;
    //f = &a;
    //g = d;
    //g = e;
    //g = f;
    //g = &a;
}

int main(int argc, char *argv[]) {
    int a = 0;
    foo(a, a, a, &a, &a, &a, &a);
    return 0;
}
