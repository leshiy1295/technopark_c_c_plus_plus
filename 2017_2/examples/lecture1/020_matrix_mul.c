#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define RETRIES 100
#define N 200

void mul1(double A[N][N], double B[N][N], double C[N][N]) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void mul2(double A[N][N], double B[N][N], double C[N][N]) {
    double transpB[N][N];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            transpB[i][j] = B[j][i];
        }
    }
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * transpB[j][k];
            }
        }
    }
}

void fill_random(double A[N][N]) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            A[i][j] = rand();
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    double A[N][N];
    double B[N][N];

    double time = 0;
    for (size_t i = 0; i < RETRIES; ++i) {
        fill_random(A);
        fill_random(B);
        double C[N][N];
        clock_t begin = clock();
        mul1(A, B, C);
        clock_t end = clock();
        time += (double)(end - begin) / CLOCKS_PER_SEC;
    }
    double avg_time = time / RETRIES;
    printf("First method - average time: %f\n", avg_time);

    time = 0;
    for (size_t i = 0; i < RETRIES; ++i) {
        fill_random(A);
        fill_random(B);
        double C[N][N];
        clock_t begin = clock();
        mul2(A, B, C);
        clock_t end = clock();
        time += (double)(end - begin) / CLOCKS_PER_SEC;
    }
    avg_time = time / RETRIES;
    printf("Second method - average time: %f\n", avg_time);
    return 0;
}
