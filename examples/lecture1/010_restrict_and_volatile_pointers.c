#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int N = 10;
    int k = 4;
    int result1 = 0;
    volatile int result2 = 0;
    // restrict int result3 = 0; // Compilation error - only porinters can be restricted
    int *volatile a = (int *)malloc(N * sizeof(int));
    if (a) {
        result1 = a[k];
        result1 = a[k]; // Shouldn't be removed, because of a is volatile
        free(a);
    }
    int *restrict b = (int *)malloc(N * sizeof(int));
    if (b) {
        result1 = b[k];
        result1 = b[k]; // Can be optimized, because b is restrict
        free(b);
    }
    int *c = (int *)malloc(N * sizeof(int));
    if (c) {
        result1 = c[k];
        result1 = c[k]; // ?, but by default - pessimistic strategy
        free(c);
    }
    result1 = result2;
    result1 = result2; // Should not be removed, because result2 is volatile
    printf("result1: %d, result2: %d\n", result1, result2);
    return 0;
}
