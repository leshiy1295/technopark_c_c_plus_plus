#include <iostream>

int main(int argc, char *argv[]) {
    int i = 0;
    int b[10000] = {0};
    /* BAD - i will not change */
    int j = sizeof(i = 1234);
    std::cout << i << " " << j << std::endl;
    j = sizeof(b[i++]);
    std::cout << i << " " << j << std::endl;

    /* GOOD */
    i = 1234; j = sizeof(i);
    std::cout << i << " " << j << std::endl;
    i++; j = sizeof(b[i]);
    std::cout << i << " " << j << std::endl;
    return 0;
}
