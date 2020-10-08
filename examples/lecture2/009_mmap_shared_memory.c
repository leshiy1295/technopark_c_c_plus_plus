#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    size_t page_size = getpagesize();

    char *shared_memory = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (!shared_memory) {
        printf("Failed to map\n");
        return 1;
    }

    int v = 0;
    *shared_memory = 0;

    pid_t pid = fork();

    if (pid == -1) {
        printf("Fork failed\n");
        if (munmap(shared_memory, page_size)) {
            printf("Failed to unmap\n");
        }
        return 1;
    }

    if (pid == 0) {
        *shared_memory = 42;
        v = 50;
    } else {
        pid_t wait_result = wait(NULL);
        if (wait_result != pid) {
            printf("Wait failed with code %d\n", wait_result);
        }
    }

    if (pid == 0) {
        printf("CHILD: Shared %d, not shared %d\n", *shared_memory, v);
    } else {
        printf("PARENT: Shared %d, not shared %d\n", *shared_memory, v);
    }

    if (pid) {
        if (munmap(shared_memory, page_size)) {
            printf("Failed to unmap\n");
        }
    }
}