#include <iostream>
#include <ctime>
#include <string>
#include <string.h>

struct Sample {
private:
    int int_prm;
    double dbl_prm;
public:
    // transformation constructors
    Sample(const char *s) {
        int_prm = strlen(s);
        dbl_prm = std::time(NULL);
        std::cout << "Sample(const char *s)" << std::endl;
    }
    Sample(const std::string &s) {
        int_prm = s.length();
        dbl_prm = std::time(NULL);
        std::cout << "Sample(std::string &s)" << std::endl;
    }
    // transformation operations
    operator int() {
        std::cout << "int()" << std::endl;
        return int_prm;
    }
    operator double() {
        std::cout << "double()" << std::endl;
        return dbl_prm;
    }

    operator char *() {
        std::cout << "char *()" << std::endl;
        std::time_t time = double(*this);
        return std::asctime(std::localtime(&time));
    }
};

int main(int argc, char *argv[]) {
    const char *s = "Hello";
    std::string ss(s);
    Sample s1(s);
    Sample s2(ss);
    int len = s1;
    for (int i = 0; i < len; ++i) {
        std::cout << s[i];
    }
    std::cout << std::endl;
    std::cout << (char *)s2 << std::endl;
    return 0;
}
