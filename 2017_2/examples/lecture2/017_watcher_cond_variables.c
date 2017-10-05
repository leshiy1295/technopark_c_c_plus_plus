/*
 * Описание:
 * Главный поток создаёт три потока. Два из них увеличивают
 * значение переменной count, в то время как третий тред
 * отслеживает значение этой переменной. Когда count
 * досгитает предопределённого лимита, ожидающий тред
 * получает сигнал от одного из инкрементирующих тредов?
 * Ожидающий тред просыпается и затем модифицирует count.
 * Программа продолжается до тех пор, пока count не станет
 * равным TCOUNT.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3
#define TCOUNT 10
#define COUNT_LIMIT 12

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t) {
    int i;
    long my_id = (long)t;

    for (i = 0; i < TCOUNT; ++i) {
        pthread_mutex_lock(&count_mutex);
        ++count;

        /*
         * Проверяем значение переменной и посылаем сигнал
         * ожидающему треду, когда условие достигнуто.
         * Важно, что это происходит в то время, когда мьютекс заблокирован.
         */
        if (count == COUNT_LIMIT) {
            printf("inc_count(): thread %ld, count = %d Treshold reached. ", my_id, count);
            pthread_cond_signal(&count_threshold_cv);
            printf("Just send signal.\n");
        }
        printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
        pthread_mutex_unlock(&count_mutex);

        /* Передача управления другим тредам */
        sleep(1);
    }
    pthread_exit(NULL);
}

void *watch_count(void *t) {
    long my_id = (long)t;

    printf("Starting watch_count(): thread %ld\n", my_id);

    /*
     * Блокировка мьютекса и ожидание сигнала. Важно, что
     * pthread_cond_wait поток автоматически и атомарно разблокирует
     * мьютекс, пока он ждёт. Также важно, что если COUNT_LIMIT
     * достигается перед вызовом этого потока ожидающим тредом,
     * цикл будет пропущен для того, чтобы предотвратить вечное ожидание
     * в потоке pthread_cond_wait.
     */
    pthread_mutex_lock(&count_mutex);
    while (count < COUNT_LIMIT) {
        printf("watch_count(): thread %ld Count= %d. Going into wait...\n", my_id, count);
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
        printf("watch_count(): thread %ld Condition signal received. Count= %d\n", my_id, count);
    }
    printf("watch_count(): thread %ld Updating the value of count...\n", my_id);
    count += 125;
    printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
    printf("watch_count(): thread %ld Unlocking mutex.\n", my_id);
    pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int i;
    long threads_ids[NUM_THREADS] = {1, 2, 3};
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    /* Инициализация мьютекса и условной переменной */
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &attr, watch_count, (void *)threads_ids[0]);
    pthread_create(&threads[1], &attr, inc_count, (void *)threads_ids[1]);
    pthread_create(&threads[2], &attr, inc_count, (void *)threads_ids[2]);

    /* Ожидание завершения всех потоков */
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    printf("Main(): Waited and joined with %d threads. Final value of count = %d. Done.\n", NUM_THREADS, count);

    /* Освобождение использованных объектов */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
    pthread_exit(NULL);
}
