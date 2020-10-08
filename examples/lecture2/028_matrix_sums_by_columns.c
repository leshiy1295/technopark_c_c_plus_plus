#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define RETRIES 100
#define N 1000

void sum1(double A[N][N], double S[N]) {
    for (size_t j = 0; j < N; ++j) {
        for (size_t i = 0; i < N; ++i) {
            S[j] += A[i][j];
        }
    }
}

void sum2(double A[N][N], double S[N]) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            S[j] += A[i][j];
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

    double time = 0;
    for (size_t i = 0; i < RETRIES; ++i) {
        fill_random(A);
        double S[N] = {0};
        clock_t begin = clock();
        sum1(A, S);
        clock_t end = clock();
        time += (double)(end - begin) / CLOCKS_PER_SEC;
    }
    double avg_time = time / RETRIES;
    printf("First method - average time: %f\n", avg_time);

    time = 0;
    for (size_t i = 0; i < RETRIES; ++i) {
        fill_random(A);
        double S[N] = {0};
        clock_t begin = clock();
        sum2(A, S);
        clock_t end = clock();
        time += (double)(end - begin) / CLOCKS_PER_SEC;
    }
    avg_time = time / RETRIES;
    printf("Second method - average time: %f\n", avg_time);
    return 0;
}
