#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *my_strdup(const char *src)
{
    size_t len = strlen(src);
    char *dest = malloc(len);
    memcpy(dest, src, len);
    dest[len] = '\0';
    return dest;
}

int main(void)
{
    const char hello[] = "Hello, world!";
    char *copy = my_strdup(hello);
    free(copy);
    printf("Result: '%s'\n", copy);
    free(copy);
    return 0;
}
