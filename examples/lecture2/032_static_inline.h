#include <stdio.h>

static inline void foo() {
    printf("foo()\n");
}

inline void foo2() {
    printf("foo2()\n");
}