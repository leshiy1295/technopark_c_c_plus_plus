#include <stdio.h>
#include <stdlib.h>

struct first {
    int id;
    char name[15];
    double amount;
    _Bool active;
};

struct second {
    int id;
    char name[15];
    double amount;
    _Bool active;
};

struct third {
    int id;
    char name[15];
    _Bool active;
    double amount;
};

int main(int argc, char *argv[]) {
    printf("Components:\nint: %ld\nchar[15]: %ld\ndouble: %ld\n_Bool: %ld\n", sizeof(int), sizeof(char[15]), sizeof(double), sizeof(_Bool));
    printf("Summary:\ns1: %ld\ns2: %ld\ns3: %ld\n", sizeof(struct first), sizeof(struct second), sizeof(struct third));
    return 0;
}
