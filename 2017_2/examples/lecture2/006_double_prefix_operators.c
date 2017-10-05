#include <stdio.h>

int main(int argc, char *argv[]) {
    int a = 5;
    printf("%d, %d\n", !!a, ~~a);
    int b = 5 + !!a;
    int c = 5 - ~~b;
    if (!!a && ~~b) {
        printf("%d\n", c);
    }
    return 0;
}
