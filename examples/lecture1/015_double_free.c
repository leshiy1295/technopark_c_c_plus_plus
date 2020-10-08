#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

char *get_new_s(char *s) {
    char *new_s = (char *)malloc((strlen(s) + 1) * sizeof(char));
    strcpy(new_s, s);
    new_s[0] = s[0] > 'a' && s[0] < 'z' ? s[0] - 'a' + 'A' : s[0];
    free(s);
    return new_s;
}

int main(int argc, char *argv[]) {
    int *null_p = NULL;
    free(null_p); // It's ok, no error
    free(null_p); // It's ok, no error

    /* Example 1 */
    char *s = (char *)malloc((10 + 1) * sizeof(char));
    if (!s) {
        return 0;
    }
    scanf("%10s", s);
    char *new_s = get_new_s(s);
    free(new_s);
    //free(s);

    /* Example 2 */
    char *a = (char *)malloc(10 * sizeof(char));
    if (a) {
        char *b = realloc(a, 100);
        if (b) {
            //free(a);
            free(b);
        }
    }

    /* Example 3 */
    char *s1 = (char *)malloc(10 * sizeof(char));
    if (!s1) {
        return 0;
    }
    char *s2 = (char *)malloc(20 * sizeof(char));
    if (!s2) {
        free(s1);
        // return 0; // forgot to return..
    }
    else {
        /* ... */
    }
    if ((uint64_t)s2 % 128 != 0) { // for some reason
        free(s2);
    }
    else {
        /* ... */
    }

    free(s1);
    free(s2);
    return 0;
}
