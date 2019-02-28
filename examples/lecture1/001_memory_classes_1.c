#include "001_memory_classes_1.h"
#include <stdio.h>

// Static variable inside file with inner linking
static const char *SECRET_WORD = "secret";

// Static variable inside file with outer linking
const char *NOT_SECRET_WORD = "not secret";

// Static function with inner linking
static void bar() {
    printf("%s\n", NOT_SECRET_WORD);
}

// Static function inside file with outer linking
void foo() {
    // Static without linking inside block
    static int v = 0;
    printf("%d\n", v++);
}

/*int main() {
    foo();
    bar();
    printf("%s\n", SECRET_WORD);
    printf("%s\n", NOT_SECRET_WORD);
    return 0;
}*/
