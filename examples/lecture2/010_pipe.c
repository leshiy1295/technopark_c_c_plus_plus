#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 80

int main() {
    char str[] = "Simple string\n";
    char buf[MAX_LEN];

    int fd[2];
    // fd[0] - reading
    // fd[1] - writing

    if (pipe(fd) != 0) {
        printf("Opening pipe failed\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        printf("Fork failed\n");
        if (close(fd[0])) {
            printf("Failed to close read pipe\n");
        }
        if (close(fd[1])) {
            printf("Failed to close write pipe\n");
        }
        return 1;
    }

    if (pid == 0) {
        if (close(fd[0])) {
            printf("CHILD: Failed to close read pipe\n");
        }
        if (write(fd[1], str, strlen(str) + 1) == -1) {
            printf("CHILD: Failed to write into pipe\n");
        }
        if (close(fd[1])) {
            printf("CHILD: Failed to close write pipe\n");
        }
        exit(0);
    }

    if (close(fd[1])) {
        printf("PARENT: Failed to close write pipe\n");
    }
    if (read(fd[0], buf, sizeof(buf)) < 0) {
        printf("PARENT: Failed to read from pipe\n");
    } else {
        printf("PARENT: Received from child: %s\n", buf);
    }
    if (close(fd[0])) {
        printf("PARENT: Failed to close read pipe\n");
    }
}