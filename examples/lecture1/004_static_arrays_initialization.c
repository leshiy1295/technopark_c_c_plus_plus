#include <stdio.h>

int main(int argc, char *argv[]) {
    int a[] = {1, 2, 3};
    int b[5] = {1, 2, 3};
    int c[7] = {1, [5] = 10, 20, [1] = 2};
    int d[10] = {[0 ... 9] = -1}; // Extension to GCC compiler
    for (size_t i = 0; i < sizeof(a) / sizeof(*a); ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
    for (size_t i = 0; i < sizeof(b) / sizeof(*b); ++i) {
        printf("%d ", b[i]);
    }
    printf("\n");
    for (size_t i = 0; i < sizeof(c) / sizeof(*c); ++i) {
        printf("%d ", c[i]);
    }
    printf("\n");
    for (size_t i = 0; i < sizeof(d) / sizeof(*d); ++i) {
        printf("%d ", d[i]);
    }
    printf("\n");
    return 0;
}
