#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd;
    char *hellofifo = "/tmp/hellofifo";
    char buf[100];

    if ((fd = open(hellofifo, O_RDONLY)) == -1) {
        printf("Pipe not exists\n");
        return 1;
    }

    printf("Connected!\n");
    read(fd, buf, sizeof(buf));
    printf("Received %s\n", buf);
    close(fd);
}