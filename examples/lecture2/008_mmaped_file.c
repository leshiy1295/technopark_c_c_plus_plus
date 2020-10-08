#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    static const char *filename = "mapped_file";
    char replacing_symbol = 'A';

    /*
    FILE *f = fopen(filename, "r");
    int fd = fileno(f);
    */
    int fd = open(filename, O_RDONLY); // или O_RDWR
    struct stat st;
    stat(filename, &st);
    
    size_t file_size = st.st_size;

    // PROT_READ - чтение; PROT_WRITE - чтение/запись
    // MAP_PRIVATE - не записывать в файл;
    // MAP_POPULATE - предзагрузка файла ядром;
    // MAP_SHARED - деление с другими процессами
    char *region = mmap(NULL,
                        file_size,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_POPULATE,
                        fd,
                        0);
    if (region == MAP_FAILED) {
        printf("mmap failed\n");
        close(fd);
        return 1;
    }

    printf("What was read\n");
    write(fileno(stdout), region, file_size); // fileno(stdout) == 1

    for (size_t i = 0; i < file_size; ++i) { region[i] = replacing_symbol; }

    printf("After modification: %s\n", region);

    if (munmap(region, file_size) != 0) {
        printf("munmap failed\n");
    }

    close(fd);
}