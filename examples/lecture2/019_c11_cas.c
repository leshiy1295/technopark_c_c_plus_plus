#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

_Atomic int lock = 0;
int lock_count = 0;
int unlock_count = 0;

void *counting(void *input) {
    int expected = 0;
    for(int i = 0; i < 100000; i++)
    {
        unlock_count++;
        while(!atomic_compare_exchange_weak(&lock,&expected,1)) //if the lock is 0(unlock), then set it to 1(lock).
            expected = 0; //if the CAS fails, the expected will be set to 1, so we need to change it to 0 again.
        lock_count++;
        lock = 0;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[10];

    for(int i = 0; i < 10; i++) {
        pthread_create(&tid[i], NULL, counting, NULL);
    }
    for(int i = 0; i < 10; i++)
        pthread_join(tid[i], NULL);

    printf("the value of lock_count is %d\n",lock_count);
    printf("the value of unlock_count is %d\n",unlock_count);

    return 0;
}
