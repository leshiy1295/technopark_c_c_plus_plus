#include <stdio.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex; // mutex for value protection
    int value; // protected value
} data_t;

// common object; mutex is initialized statically
data_t data = {PTHREAD_MUTEX_INITIALIZER, 0};

void *thread_routine(void *arg) {
    pthread_mutex_t *mutex = &data.mutex;
    int errflag = 0;
    // lock mutex before value change
    errflag = pthread_mutex_lock(mutex);
    // check if pthread_mutex_lock call was successful
    if (errflag != 0) {
        // ...
    }
    data.value += 1; // change value
    // unlock mutex after value change
    errflag = pthread_mutex_unlock(mutex);
    // check if pthread_mutex_unlock call was successful
    if (errflag != 0) {
        // ...
    }
    return arg;
}

int main(int argc, char *argv[]) {
    const int N = 10000;
    pthread_t threadIds[N];
    for (int i = 0; i < N; i++) {
        int errflag = pthread_create(&threadIds[i], NULL, thread_routine, NULL);
        if (errflag != 0) {
            // ...
        }
    }
    for (int i = 0; i < N; i++) {
        int errflag = pthread_join(threadIds[i], NULL);
        if (errflag != 0) {
            // ...
        }
    }
    printf("result value: %d\n", data.value);
    return 0;
}
