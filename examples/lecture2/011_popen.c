#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 80

// try ARGS="\"ls\" \"sort -r\""
int main(int argc, char *argv[]) {
    FILE *pipe_in, *pipe_out;
    char read_buf[MAX_LEN];

    if ((pipe_in = popen(argv[1], "r")) == NULL) {
        printf("Failed to create pipe_in\n");
        exit(1);
    }

    if ((pipe_out = popen(argv[2], "w")) == NULL) {
        printf("Failed to create pipe_out\n");
        if (pclose(pipe_in)) {
            printf("Failed to close pipe_in\n");
        }
        exit(1);
    }

    while (fgets(read_buf, MAX_LEN, pipe_in)) {
        printf("%s", read_buf);
        fputs(read_buf, pipe_out);
    }    

    if (pclose(pipe_in)) {
        printf("Failed to close pipe_in\n");
    }

    if (pclose(pipe_out)) {
        printf("Failed to close pipe_out\n");
    }

    return 0;
}
