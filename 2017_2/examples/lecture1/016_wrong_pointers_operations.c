#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // bzero
#include <string.h>

int main(int argc, char *argv[]) {
    //memcpy(NULL, argv[0], strlen(argv[0])); // ignoring nonnull
    char *s = (char *)malloc(10 * sizeof(char));
    if (!s) {
        exit(1);
    }
    bzero(s, 0);
    char *s2 = (char *)malloc(10 * sizeof(char));
    if (!s2) {
        //free(s); // Don't forget to free even with exit...
        exit(1); // Don't use exit, system etc.. Don't use qsort, for instance, because it takes user function, whick can be unpredictable
    }
    char buf[100];
    //free(s); // Don't forget to free dynamically allocated memory
    s = realloc(s, 0); // s may become NULL
    free(s2);
    printf("&s: %p\n", s);
    //printf("*s: %c", *s); // NULL pointers should not be dereferenced
    //free(buf); // Don't free stack memory
    return 0;
}
