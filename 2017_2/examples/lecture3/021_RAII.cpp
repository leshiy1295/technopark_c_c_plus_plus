#include <iostream>
#include <time.h>
#include <stdlib.h>

bool cond() {
    return rand() % 2 == 0;
}

void logic(const char *s) {}

void bad() {
    std::cout << "ALLOCATION" << std::endl;
    char *str = new char[10];
    if (cond()) {
        std::cout << "EMERGENCY RETURN" << std::endl;
        std::cout << "LEAK :-(" << std::endl;
        return;
    }
    logic(str);
    std::cout << "CLEANING" << std::endl;
    delete []str;
}

class RAII_str {
private:
    char *str;
public:
    RAII_str(char *str) : str(str) {
        std::cout << "ALLOCATION" << std::endl;
        std::cout << "RAII_str()" << std::endl;
    }

    ~RAII_str() {
        std::cout << "~RAII_str()" << std::endl;
        std::cout << "CLEANING" << std::endl;
        delete []str;
    }

    const char *getStr() {
        return str;
    }
};

void good() {
    RAII_str r(new char[10]);
    if (cond()) {
        std::cout << "EMERGENCY RETURN" << std::endl;
        return;
    }
    logic(r.getStr());
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    bad();
    good();
    return 0;
}
