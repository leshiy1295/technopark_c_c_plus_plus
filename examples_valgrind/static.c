#include <stdio.h>

char dest[2];

int main(void)
{
    char src[2];
    src[0] = 'a';
    src[1] = 'b';
    for (int i = 0; i <= 2; i++)
        dest[i] = src[i];
    printf("%s\n", dest);
    return 0;
}
