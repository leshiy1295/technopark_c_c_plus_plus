#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int balance;
    pthread_mutex_t balance_mutex;
} bank_account;

typedef struct {
    bank_account *from;
    bank_account *to;
    int amount;
} deposit_thr_args;

void create_bank_account(bank_account **ba, int initial_amount) {
    int result = 0;
    bank_account *nba = (bank_account *)malloc(sizeof(bank_account));
    if (nba == NULL) {
        /* Handle error */
    }

    nba->balance = initial_amount;
    result = pthread_mutex_init(&nba->balance_mutex, NULL);
    if (result) {
        /* Handle error */
    }

    *ba = nba;
}

void *deposit(void *ptr) {
    int result = 0;
    deposit_thr_args *args = (deposit_thr_args *)ptr;
    for (;;) {
        printf("FROM: %p, BEFORE LOCKING %p\n", args->from, args->from);
        if ((result = pthread_mutex_lock(&(args->from->balance_mutex))) != 0) {
            /* Handle error */
        }
        printf("FROM: %p, AFTER LOCKING %p\n", args->from, args->from);

        sleep(2);
        /* Not enough balance to transfer */
        if (args->from->balance < args->amount) {
            printf("FROM: %p, BEFORE UNLOCKING %p\n", args->from, args->from);
            if ((result = pthread_mutex_unlock(&(args->from->balance_mutex))) != 0) {
                /* Handle error */
            }
            printf("FROM: %p, AFTER UNLOCKING %p\n", args->from, args->from);
            return NULL;
        }

        printf("FROM: %p, BEFORE TRY LOCKING %p\n", args->from, args->to);
        if ((result = pthread_mutex_trylock(&(args->to->balance_mutex))) != 0) {
            printf("%p IS LOCKED. UNLOCKING %p\n", args->to, args->from);
            printf("FROM: %p, BEFORE UNLOCKING %p\n", args->from, args->from);
            if ((result = pthread_mutex_unlock(&(args->from->balance_mutex))) != 0) {
                /*  Handle error */
            }
            printf("FROM: %p, AFTER UNLOCKING %p\n", args->from, args->from);
            sleep(2);
        }
        else {
            printf("FROM: %p, AFTER TRY LOCKING %p\n", args->from, args->to);
            break;
        }
    }

    args->from->balance -= args->amount;
    args->to->balance += args->amount;

    printf("FROM: %p, BEFORE UNLOCKING %p\n", args->from, args->from);
    if ((result = pthread_mutex_unlock(&(args->from->balance_mutex))) != 0) {
        /*  Handle error */
    }
    printf("FROM: %p, AFTER UNLOCKING %p\n", args->from, args->from);
    printf("FROM: %p, BEFORE UNLOCKING %p\n", args->from, args->to);
    if ((result = pthread_mutex_unlock(&(args->to->balance_mutex))) != 0) {
        /* Handle error */
    }
    printf("FROM: %p, AFTER UNLOCKING %p\n", args->from, args->to);

    free(ptr);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thr1, thr2;
    int result = 0;

    bank_account *ba1 = NULL;
    bank_account *ba2 = NULL;
    create_bank_account(&ba1, 1000);
    create_bank_account(&ba2, 1000);

    deposit_thr_args *arg1 = (deposit_thr_args *)malloc(sizeof(deposit_thr_args));
    if (arg1 == NULL) {
        /* Handle error */
    }
    deposit_thr_args *arg2 = (deposit_thr_args *)malloc(sizeof(deposit_thr_args));
    if (arg2 == NULL) {
        /* Handle error */
    }

    arg1->from = ba1;
    arg1->to = ba2;
    arg1->amount = 100;

    arg2->from = ba2;
    arg2->to = ba1;
    arg2->amount = 100;

    /* Perform the deposits */
    if ((result = pthread_create(&thr1, NULL, deposit, (void *)arg1)) != 0) {
        /* Handle error */
    }
    if ((result = pthread_create(&thr2, NULL, deposit, (void *)arg2)) != 0) {
        /* Handle error */
    }

    pthread_exit(NULL);
    return 0;
}
