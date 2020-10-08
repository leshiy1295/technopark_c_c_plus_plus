#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex; // mutex for value change protection
    pthread_cond_t cond; // conditional variable for communication
    int value; // protected value
} data_t;

// common object: mutex and conditional variable
// static initialization
data_t data = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0};

void *thread_routine(void *arg) {
    int errflag = pthread_mutex_lock(&data.mutex);
    if (errflag != 0) {
        // ...
    }
    data.value = 1; // change value
    // send signal about value change
    errflag = pthread_cond_signal(&data.cond);
    if (errflag != 0) {
        // ...
    }
    errflag = pthread_mutex_unlock(&data.mutex);
    if (errflag != 0) {
        // ...
    }
    return arg; // another variant: return NULL etc
}

int main(int argc, char *argv[]) {
    int errflag = 0;
    struct timespec timeout;
    pthread_t thread;
    errflag = pthread_create(&thread, NULL, thread_routine, NULL);
    if (errflag != 0) {
        // ...
    }
    timeout.tv_sec = time(NULL) + 1;
    timeout.tv_nsec = 0;

    // blocking thread on conditional variable
    // requires prior mutex lock
    errflag = pthread_mutex_lock(&data.mutex);
    if (errflag != 0) {
        // ...
    }
    while (data.value == 0) { // first predicate check
        errflag = pthread_cond_timedwait(&data.cond, &data.mutex, &timeout);
        if (errflag == ETIMEDOUT) {
            break; // unlocking by timeout
        }
        else {
            // error on pthread_cond_timedwait call
        }
    }
    if (data.value != 0) { // second predicate check
        // ...
    }

    errflag = pthread_mutex_unlock(&data.mutex);
    if (errflag != 0) {
        // ...
    }
    errflag = pthread_join(thread, NULL);
    if (errflag != 0) {
        // ...
    }
    printf("result value: %d\n", data.value);
    return EXIT_SUCCESS;
}
