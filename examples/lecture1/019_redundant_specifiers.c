#include <stddef.h> // NULL declaration

int main(int argc, char *argv[]) {
    const int const *bad_v1a = NULL; // both specifiers apply to int
    const int const *bad_v1b = NULL;
    static static int bad_v2 = 0;

    const int *v1a = NULL;
    int const * const v1b = NULL;
    static int v2 = 0;
    return 0;
}
