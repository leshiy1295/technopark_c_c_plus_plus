#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();

    if (pid == -1) {
        printf("fork failed\n");
        return 1;
    }

    if (pid == 0) {
        sleep(1);
        printf("CHILD: Hello world\n");
        exit(0);
    }

    printf("PARENT: forked child with pid %d\n", pid);

    return 0;
}