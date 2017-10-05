#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define unlikely(expr) __builtin_expect(!!(expr), 0)
#define likely(expr) __builtin_expect(!!(expr), 1)

void foo() {
    printf("foo()\n");
}

void bar() {
    printf("bar()\n");
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int a = rand() % 10;

    if (unlikely(a > 8)) // false in 80% of cases
        foo();
    else
        bar();
    return 0;
}
