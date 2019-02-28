// To run example one need to compile two .c files together
#include "001_memory_classes_1.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Automatic memory class
    auto int i = 0;
    int j = 1;
    // Register memory class
    register int k = 1;
    foo();
    foo();
    //bar(); // Compilation error - implementation not found
    //printf("%s\n", SECRET_WORD); // Runtime error - value not found
    printf("%s\n", NOT_SECRET_WORD);
    return 0;
}
