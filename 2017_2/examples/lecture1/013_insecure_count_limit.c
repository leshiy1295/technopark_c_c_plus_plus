#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <bsd/string.h> // for strlcpy. Prior to run on should run install libbsd-dev package on Ubuntu like OS, and run with -lbsd flag

#define MAX_STR_LEN 80

void foo(char *s) {
    char *local = (char *)malloc((MAX_STR_LEN + 1) * sizeof(char));
    if (!local) {
        return;
    }
    // NON-COMPLIANT
    //strcpy(local, s);
    //sprintf(local, "%s", s);

    // COMPLIANT
    //snprintf(local, MAX_STR_LEN, "%s", s);
    //strlcpy(local, s, MAX_STR_LEN);
    strncpy(local, s, MAX_STR_LEN);
    local[MAX_STR_LEN] = '\0';
    printf("From foo: %s\n", local);
    free(local);
}

int main(int argc, char *argv[]) {
    int N = 100;
    char *s = (char *)malloc((N + 1) * sizeof(char));
    if (!s) {
        return 0;
    }
    for (size_t i = 0; i < N; ++i) {
        s[i] = 'A';
    }
    s[N] = '\0';
    printf("From main: %s\n", s);
    foo(s);
    free(s);
    return 0;
}
