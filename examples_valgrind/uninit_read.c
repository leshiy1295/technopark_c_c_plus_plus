#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    char *data = malloc(5);
    data[0] = 'a';
    data[1] = 'b';
    data[1] = 'c';
    printf("data[2] is equal to 0x%02x\n", data[2]);
    if (data[2] == 'c')
        printf("It is 'c'\n");
    else
        printf("It is not 'c'\n");
    free(data);
    return 0;
}

