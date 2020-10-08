#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore;

void *thread_func(void *arg) {
    int id = (int)arg;
    for (int i = 0; i < 3; ++i) {
        printf("THREAD #%d: Waiting semaphore (i = %d)...\n", id, i);
        sem_wait(&semaphore);
        printf("THREAD #%d: Hello (i = %d)\n", id, i);
        sleep(2);
        sem_post(&semaphore);
        printf("THREAD #%d: Leaving critical area (i = %d)...\n", id, i);
        sleep(2);
    }
    printf("THREAD #%d: Finished work!\n", id);
    return arg;
}

int main(int argc, char *argv[]) {
    const int threads_count = 3;
    // Инициализация семафора:
    // первый параметр = 0 => семафор используется между тредами процесса и доступен всем тредам
    // второй параметр - лимит одновременно находящихся в критической зоне тредов
    pthread_t threads[threads_count];
    sem_init(&semaphore, 0, 1);

    for (int i = 0; i < threads_count; ++i) {
        printf("Starting thread #%d\n", i);
        pthread_create(&threads[i], NULL, thread_func, (void *)i);
    }

    for (int i = 0; i < threads_count; ++i) {
        void *result = NULL;
        pthread_join(threads[i], &result);
        printf("Registered end of thread #%d\n", (int)result);
    }
    return 0;
}
