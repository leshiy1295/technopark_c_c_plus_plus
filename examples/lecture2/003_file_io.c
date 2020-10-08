#include <stdio.h>

int main() {
    struct book {
        char name[20];
        size_t pages_count;
    } my_book = { "Tutorial", 100 };

    static const char *file_name = "books";

    // Write to file
    FILE *f = fopen(file_name, "wb");

    if (!f) {
        fprintf(stderr, "Failed to open file for write\n");
        return 1;
    }

    if (fwrite(&my_book, sizeof(struct book), 1, f) != 1) {
        fprintf(stderr, "Failed to write struct into file\n");
        // Intentional no return to close file
    }

    if (fclose(f)) {
        fprintf(stderr, "Failed to close file\n");
        return 1;
    }

    // Reading file
    if ((f = fopen(file_name, "rb")) == NULL) {
        fprintf(stderr, "Failed to open file for read\n");
        return 1;
    }

    while (!feof(f)) {
        if (fread(&my_book, sizeof(struct book), 1, f) == 1) {
            printf("Book %s (%ld pages)\n", my_book.name, my_book.pages_count);
        }
    }

    if (fclose(f)) {
        fprintf(stderr, "Failed to close file\n");
        return 1;
    }
}
