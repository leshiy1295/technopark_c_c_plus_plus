#include <stdio.h>

int pow1(int a, int b) {
    printf("pow1()\n");
    if (b == 0) {
        return 0;
    }
    int x = a;
    for (int i = 1; i < b; i++) {
        x = x * a; // Dead store because the last return
                   // statement should return x instead
                   // of returning a
    }
    return a;
}

int pow2(int a, int b) {
    printf("pow2()\n");
    if (b == 0) {
        return 0;
    }
    int x = a;
    for (int i = 1; i < b; i++) {
        x = x * a;
    }
    return x;
}

int main(int argc, char *argv[]) {
    printf("%d\n", pow1(2, 4));
    printf("%d\n", pow2(2, 4));
    return 0;
}
