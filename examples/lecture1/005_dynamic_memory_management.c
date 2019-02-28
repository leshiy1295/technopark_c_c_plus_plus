#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 10

int main(int argc, char *argv[]) {
    int *a = (int *)malloc(SIZE * sizeof(int));
    /*if (a) {
        for (size_t i = 0; i < SIZE; ++i) {
            printf("%d ", a[i]);
        }
    }*/

    printf("\n");
    int *b = (int *)calloc(SIZE, sizeof(int));
    if (b) {
        for (size_t i = 0; i < SIZE; ++i) {
            printf("%d ", b[i]);
        }
    }

    printf("\n");
    char *str_a = (char *)malloc((SIZE + 1) * sizeof(char)); // One more for '\0'
    /*if (str_a) {
        printf("uninitialized malloc string (%s) length: %ld\n", str_a, strlen(str_a));
        str_a[0] = '\0'; // String must terminate with '\0'
        printf("initialized string length: %ld\n", strlen(str_a));
    }*/

    char *str_b = (char *)calloc((SIZE + 1), sizeof(char)); // One more for '\0'
    if (str_b) {
        printf("uninitialized calloc string (%s) length: %ld\n", str_b, strlen(str_b));
        str_b[0] = '\0'; // String must terminate with '\0'
        printf("initialized string length: %ld\n", strlen(str_b));
    }

    free(a);
    printf("a after free: %p\n", a);
    a = NULL;

    int *new_b = realloc(b, 0);
    printf("b realloc: %p -> %p\n", b, new_b);
    // free(b);
    b = NULL;

    int *new_a = realloc(a, 2 * SIZE * sizeof(int));
    printf("a realloc: %p -> %p\n", a, new_a);
    /*if (new_a) {
        for (size_t i = 0; i < 2 * SIZE; ++i) {
            printf("%d ", new_a[i]);
        }
    }*/
    printf("\n");

    if (new_a) {
        free(new_a);
    }
    if (new_b) {
        free(new_b);
    }
    if (str_a) {
        free(str_a);
    }
    if (str_b) {
        free(str_b);
    }
    return 0;
}
