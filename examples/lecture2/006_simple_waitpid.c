#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int pid, status;
    pid = fork();

    if (pid == 0) {
        sleep(2);
        exit(EXIT_SUCCESS);
    }

    printf("Forked with pid %d\n", pid);

    do {
        pid_t waited_pid = waitpid(pid, &status, WNOHANG);

        printf("Got waited_pid=%d, status %d\n", waited_pid, status);

        if (waited_pid < 0) {
            printf("waitpid error %d\n", waited_pid);
            break;
        }

        if (waited_pid) {
            if (WIFEXITED(status)) {
                printf("Exited with code %d\n", WEXITSTATUS(status));
            } 
            else if (WIFSIGNALED(status)) {
                printf("Killed by signal %d\n", WTERMSIG(status));
            }
            else if (WIFSTOPPED(status)) {
                printf("Stopped by signal %d\n", WSTOPSIG(status));
            }
            break;
        }

        sleep(1);
    } while (1);
}