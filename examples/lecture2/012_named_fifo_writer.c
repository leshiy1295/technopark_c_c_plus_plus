#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    int fd;
    char *hellofifo = "/tmp/hellofifo";

    unlink(hellofifo);

    if (mkfifo(hellofifo, 0777) != 0) {
        printf("Failed to create named fifo\n");
        return 1;
    }

    printf("Waiting for reader...\n");
    fd = open(hellofifo, O_WRONLY);
    printf("Reader found!\n");
    sleep(10);

    static const char message[] = "Hello reader!";
    
    write(fd, message, sizeof(message));
    printf("Message %s sent\n", message);
    close(fd);

    if (unlink(hellofifo)) {
        printf("Failed to remove named fifo\n");
    }
}