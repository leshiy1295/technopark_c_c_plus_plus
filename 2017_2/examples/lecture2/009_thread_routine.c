#include <stdlib.h> // EXIT_SUCCESS
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define MAX_SLEEP_TIME 5

void rand_sleep(char *whoami) {
    int sleep_time = 1 + rand() % MAX_SLEEP_TIME;
    printf("%s: sleeping for %d seconds...\n", whoami, sleep_time);
    sleep(sleep_time);
}

void *thread_routine(void *arg) {
    int errflag = 0;
    // ...
    // detach from main process as a POSIX thread until the end
    errflag = pthread_detach(pthread_self());
    // check if pthread_detach() call was successful
    if (errflag != 0) {
        printf("Thread: caught error: %d\n", errflag);
    }
    rand_sleep("Thread");
    printf("Thread: hello!\n");
    // gracefully exit thread with void *value as a result
    return arg; // another variant: return NULL etc
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    int errflag = 0;
    pthread_t thread; // thread id

    // create and tun POSIX thread
    errflag = pthread_create(&thread, NULL, thread_routine, NULL);
    // check if thread_create call was successful
    if (errflag != 0) {
        printf("Main: caught error: %d\n", errflag);
    }
    rand_sleep("Main");
    printf("Main: thread id: %ld\n", thread);
    // pthread_exit(0);
    // gracefully exit main thread and associated process
    return EXIT_SUCCESS;
}
