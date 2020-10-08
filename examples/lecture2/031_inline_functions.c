#include <stdio.h>

inline __attribute__((always_inline)) void foo1() {
    printf("foo1()\n");
}

/*inline __attribute__((noinline)) void foo2() {
    printf("foo2()\n");
}*/

__attribute__((always_inline)) void foo3() {
    printf("foo3()\n");
}

__attribute__((noinline)) void foo4() {
    printf("foo4()\n");
}

void foo5() {
    printf("foo5()\n");
}

int main(int argc, char *argv[]) {
    foo1();
    //foo2();
    foo3();
    foo4();
    foo5();
    return 0;
}
