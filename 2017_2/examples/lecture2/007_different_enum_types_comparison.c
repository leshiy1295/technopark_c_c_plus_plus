#include <stdio.h>
#include <stdbool.h>

enum apple {BRAEBURN, FUJI, GRANNY_SMITH, RED_DELICIOUS};
enum orange {BLOOD, NAVEL, BITTER, BERGAMOT, MANDARIN};

bool fun(enum apple v1, enum orange v2) {
    return v1 == v2; // Non-compliant
}

int main(int argc, char *argv[]) {
    if (fun(FUJI, NAVEL)) {
        printf("FUJI (%d) == NAVEL (%d)\n", FUJI, NAVEL);
    }
    return 0;
}
