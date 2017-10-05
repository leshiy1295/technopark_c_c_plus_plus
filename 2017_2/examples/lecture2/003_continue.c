#include <stdio.h>

void foo1() {
    printf("foo1()\n");
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            continue; // Non-compliant
        }
        printf("i = %d\n", i);
    }
}

void foo2() {
    printf("foo2()\n");
    for (int i = 0; i < 10; i++) {
        if (i != 5) { // Compliant
            printf("i = %d\n", i);
        }
    }
}

int main(int argc, char *argv[]) {
    foo1();
    foo2();
    return 0;
}
