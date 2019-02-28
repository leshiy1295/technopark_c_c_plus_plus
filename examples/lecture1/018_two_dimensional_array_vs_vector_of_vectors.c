#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    int *a;
    int **b;
    int N = 10;
    int M = 10;
    /* 2 dimensional array */
    printf("2-dim\n");
    a = (int *)malloc(N * M * sizeof(int));
    if (a) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j) {
                a[i * M + j] = i + j;
                printf("%2d ", a[i * M + j]);
            }
            printf("\n");
        }

        printf("2-dim-linear:\n");
        for (size_t i = 0; i < N * M; ++i) {
            printf("%2d ", a[i]);
        }
        printf("\n");
        free(a);
    }

    /* vector of vectors */
    b = (int **)malloc(N * sizeof(int *));
    if (b) {
        bool ok = true;
        for (size_t i = 0; i < N; ++i) {
            b[i] = (int *)malloc(M * sizeof(int));
            if (!b[i]) {
                for (size_t j = 0; j < i; ++j) {
                    free(b[j]);
                }
                ok = false;
                break;
            }
            for (size_t j = 0; j < M; ++j) {
                b[i][j] = i + j;
            }
        }
        if (ok) {
            for (size_t i = 0; i < N; ++i) {
                printf("%p - %p\n", (void *)b[i], (void *)&b[i][M - 1]);
            }
            for (size_t i = 0; i < N; ++i) {
                free(b[i]);
            }
        }
        free(b);
    }
    return 0;
}
