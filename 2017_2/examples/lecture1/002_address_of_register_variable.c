#include <stdio.h>

int main(int argc, char *argv[]) {
    register int a = 42;
    printf("Address of \"register\" variable a with value %d is %p\n", a, &a); // Compiler error
                                                                               // but in some compilers (like g++)
                                                                               // register specifier may be ignored
    return 0;
}
