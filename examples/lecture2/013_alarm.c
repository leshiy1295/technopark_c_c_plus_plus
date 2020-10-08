#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int counter = 0;

void handler(int signum) {
    printf("Handler called for signum %d\n", signum);
    if (signum == SIGALRM) {
        printf("Counter = %d\n", counter);
        alarm(1);
    }

    if (signum == SIGINT) {
        // убрать обработчик сигнала - обработка по умолчанию
        signal(signum, SIG_DFL);
    }
}

int main() {
    printf("My pid is %d\n", getpid());

    if (signal(SIGINT, handler) == SIG_ERR) {
        printf("Failed to register handler for SIGINT\n");
    }
    if (signal(SIGALRM, handler) == SIG_ERR) {
        printf("Failed to register handler for SIGALRM\n");
    }
    while (1) { ++counter; }
    return 0;
}