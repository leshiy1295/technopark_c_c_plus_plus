#include <stdio.h>

int main(int argc, char *argv[]) {
    char buf[80];
    // NON-COMPLIANT
    scanf("%s", buf);
    // COMPLIANT
    //scanf("%80s", buf);
    printf("%s\n", buf);
    return 0;
}
