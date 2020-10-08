#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("Будет выполнена программа %s...\n", argv[0]);
    printf("Выполняется %s\n", argv[0]);
    // execv(argv[0], argv);
    execl(argv[0], " ", "Hello", "World!", NULL);
    printf("system status_code=%d\n", system("ls -la >/dev/null 2>&1"));
    return 0;
}