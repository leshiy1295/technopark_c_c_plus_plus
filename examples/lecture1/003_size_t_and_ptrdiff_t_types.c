#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> // ptrdiff_t
#include <time.h>

size_t find_max(size_t *first, size_t *last) {
    ptrdiff_t elements_count = last - first;
    if (elements_count <= 0) {
        return 0;
    }
    size_t max = *first;
    for (size_t i = 1; i < elements_count; ++i) {
        if (first[i] > max) {
            max = first[i];
        }
    }
    return max;
}

int main(int argc, char *argv[]) {
    // Incorrect use
    printf("Incorrect working via pointer with incoming data\n");
    size_t total_size = sizeof argv; // wrong answer: got pointer, not static array
    size_t element_size = sizeof argv[0];
    size_t elements_count = total_size / element_size;
    printf("Expected: %d (argc), found: %ld (sizeof) arguments\n", argc, elements_count);
    for (size_t i = 0; i < elements_count; ++i) {
        printf("%s\n", argv[i]);
    }

    // Incorrect use
    printf("Incorrect working with dynamically allocated array via pointer\n");
    const int N = 10;
    int *dyn_arr = (int *)malloc(N * sizeof(int));
    total_size = sizeof dyn_arr;
    element_size = sizeof dyn_arr[0];
    elements_count = total_size / element_size;
    printf("Found %ld array elements\n", elements_count);
    free(dyn_arr);

    // Correct use
    printf("Working with statically allocated array\n");
    size_t arr[N];
    total_size = sizeof(arr);
    element_size = sizeof(arr[0]);
    elements_count = total_size / element_size;
    printf("Found %ld array elements\n", elements_count);

    // Another way to find array size
    ptrdiff_t elements_between = &arr[N] - arr;
    printf("Found %d array elements via ptrdiff_t\n", elements_between);

    srand(time(NULL));
    printf("Random array: \n");
    for (size_t i = 0; i < elements_between; ++i) {
        arr[i] = rand() % 10;
        printf("%ld ", arr[i]);
    }
    printf("\n");
    printf("correct: %ld\n", find_max(arr, &arr[N]));
    printf("incorrect: %ld\n", find_max(&arr[N], arr));

    // Incorrect diff
    char char_arr[N];
    printf("sizeof size_t is %ld\n", sizeof(size_t));
    ptrdiff_t char_less_than_one_diff = &char_arr[sizeof(size_t) - 1] - &char_arr[0];
    ptrdiff_t size_t_less_than_one_diff = (size_t *)&char_arr[sizeof(size_t) - 1] - (size_t *)&char_arr[0];
    printf("size_t_less_than_one_diff: %d (char is %d)\n", size_t_less_than_one_diff, char_less_than_one_diff);
    ptrdiff_t char_more_than_one_diff = &char_arr[sizeof(size_t) + 1] - &char_arr[0];
    ptrdiff_t size_t_more_than_one_diff = (size_t *)&char_arr[sizeof(size_t) + 1] - (size_t *)&char_arr[0];
    printf("size_t_more_than_one_diff: %d (char is %d)\n", size_t_more_than_one_diff, char_more_than_one_diff);
    return 0;
}
