#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_STR_SIZE 80

void foo(char *s) {
    char buf[MAX_STR_SIZE];
    strcpy(buf, s);
    printf("From foo: %s\n", buf);
}

int main(int argc, char *argv[]) {
    int N = 100;
    char *s = (char *)malloc((N + 1) * sizeof(char));
    for (size_t i = 0; i < N; ++i) {
        s[i] = 'A';
    }
    s[N] = '\0';
    printf("From main: %s\n", s);
    foo(s);
    return 0;
}
