#include <stddef.h>

// NON-COMPLIANT
__attribute__((returns_nonnull))
__attribute__((nonnull))
int *nonnull(int *parameter) {
    parameter = NULL; // "parameter" is marked "nonnull"
                      // but is set to null.
    return NULL;      // function's return value is marked
                      // "nonnull" but null is returned
}

int main(int argc, char *argv[]) {
    nonnull(NULL);    // "parameter" to this call is marked
                      // "nonnull", but null is passed.
    return 0;
}
