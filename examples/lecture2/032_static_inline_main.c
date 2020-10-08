#include "032_static_inline.h"

extern void foo2();
void bar();

int main() {
    foo();
    bar();
    foo2();
}