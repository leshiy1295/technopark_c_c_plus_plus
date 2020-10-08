#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 10
#define LOOPS 100000
#define RETRIES 100

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// using pthread_mutex_lock
void *lock_worker(void *arg) {
    for (size_t i = 0; i < LOOPS; ++i) {
        pthread_mutex_lock(&mutex);
        ++counter;
        pthread_mutex_unlock(&mutex);
    }
    return arg;
}

// using try_lock - obviously only use one at a time
void *trylock_worker(void *arg) {
    for (size_t i = 0; i < LOOPS; ++i) {
        while (pthread_mutex_trylock(&mutex) != 0) {
            // wait - threated as spin lock in this example
        }
        ++counter;
        pthread_mutex_unlock(&mutex);
    }
    return arg;
}

void bench(void (*f)(void)) {
    clock_t begin = clock();

    for (size_t i = 0; i < RETRIES; ++i) {
        f();
    }

    clock_t end = clock();
    double time = (double)(end - begin) / CLOCKS_PER_SEC;
    double avg_time = time / RETRIES;

    printf("Average time spent: %f\n", avg_time);

}

void bench_lock_workers() {
    pthread_t t[NUM_THREADS];
    int result = 0;

    counter = 0;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        result = pthread_create(&t[i], NULL, lock_worker, NULL);
        if (result) {
            printf("Thread #%d failed\n", (int)i);
        }
    }

    for (size_t i = 0; i < NUM_THREADS; ++i) {
        pthread_join(t[i], NULL);
    }

    //printf("%d\n", counter);
}

void bench_trylock_workers() {
    pthread_t t[NUM_THREADS];
    int result = 0;

    counter = 0;
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        result = pthread_create(&t[i], NULL, trylock_worker, NULL);
        if (result) {
            printf("Thread #%d failed\n", (int)i);
        }
    }

    for (size_t i = 0; i < NUM_THREADS; ++i) {
        pthread_join(t[i], NULL);
    }

    //printf("%d\n", counter);
}

int main(int argc, char *argv[]) {
    printf("Benchmarking lock workers...\n");
    bench(bench_lock_workers);
    printf("Benchmarking trylock workers...\n");
    bench(bench_trylock_workers);
    return 0;
}
