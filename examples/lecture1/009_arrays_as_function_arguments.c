#include <stdio.h>

#define YSIZE 2
#define ZSIZE 3

void foo(size_t, size_t, int [*][*]);
void bar(int (*)[YSIZE][ZSIZE], size_t);

void foo(size_t x, size_t y, int a[x][y]) {
    printf("foo()\n");
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

void bar(int a[][YSIZE][ZSIZE], size_t x) {
    printf("bar()\n");
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < YSIZE; ++j) {
            for (size_t k = 0; k < ZSIZE; ++k) {
                printf("%d ", a[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int N = 2;
    int M = N * N;
    int a[N][M] /* = { 0 } */; // VLA arrays can not be initialized
    printf("a\n");
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            a[i][j] = i + j;
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    foo(N, M, a);
    int L = M * N;
    int b[L][YSIZE][ZSIZE];
    printf("b\n");
    for (size_t i = 0; i < L; ++i) {
        for (size_t j = 0; j < YSIZE; ++j) {
            for (size_t k = 0; k < ZSIZE; ++k) {
                b[i][j][k] = i + j + k;
                printf("%d ", b[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    bar(b, L);
    return 0;
}
