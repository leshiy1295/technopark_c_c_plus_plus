#include <stdio.h>

#define ROWS 2
#define COLS 3

int main(int argc, char *argv[]) {
    int a[ROWS][COLS] = {
        {0, 1},
        {2, 3, 4}
    };
    int b[ROWS][COLS] = {0, 1, 2, 3, 4};

    printf("a\n");
    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLS; ++j) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("a as linear\n");
    int (*linear)[ROWS * COLS] = &a;
    for (size_t i = 0; i < ROWS * COLS; ++i) {
        printf("%d ", (*linear)[i]);
    }
    printf("\n");
    printf("\n");

    printf("b\n");
    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLS; ++j) {
            printf("%d ", b[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("b as linear\n");
    linear = &b;
    for (size_t i = 0; i < ROWS * COLS; ++i) {
        printf("%d ", (*linear)[i]);
    }
    printf("\n");

    int k[3][5];
    int (*pk)[5];
    int *p[5];

    pk = k;
    pk[0][0] = 1;
    *pk[0] = 2;
    printf("pk[0][0] = %d\n", pk[0][0]);
    **pk = 3;
    printf("pk[0][0] = %d\n", pk[0][0]);

    double *pd, **ppd;
    double (*pda)[2];
    double dbl32[3][2];
    double dbl23[2][3];

    // correct use
    pd = &dbl32[0][0];
    pd = dbl32[1];
    pda = dbl32;
    ppd = &pd;

    // incorrect use
    pd = dbl32;
    pda = dbl23;
    return 0;
}
