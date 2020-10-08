#include <stdio.h>
#include <unistd.h> // sysconf, _SC_LEVEL1_DCACHE_LINESIZE
#include <stdlib.h> // posix_memalign
#include <string.h> // memcpy
#include <stdint.h> // uint64_t

int main(int argc, char *argv[]) {
    int b[7] = {1, [5] = 10, 20, [1] = 2};
    int *p = NULL;
    int errflag;

    long l1dcls = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    if (l1dcls == -1) {
        l1dcls = sizeof(void *);
    }
    errflag = posix_memalign((void **)&p, l1dcls, sizeof b);
    if (!errflag) {
        printf("\nL1d cache line size is %ld\n", l1dcls);
        printf("p (mod L1d) and &p (mod L1d) are %p (%ld) and %p (%ld)\n", p, (uint64_t)p % l1dcls, &p, (uint64_t)&p % l1dcls);
        p = memcpy(p, b, sizeof(b));
        for (size_t i = 0; i < sizeof(b) / sizeof(b[0]); ++i) {
            printf("%d ", b[i]);
        }
        printf("\n");
        free(p);
    }
    else {
        printf("posix_memalign error: %d\n", errflag);
    }
    return 0;
}
