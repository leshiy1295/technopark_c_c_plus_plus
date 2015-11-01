# 1 "./include.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "./include.c"
# 1 "./header1.h" 1



# 1 "./header2.h" 1



extern char *a;
extern char *b;

static inline
char *foo()
{
    return a + b;
}
# 5 "./header1.h" 2
# 2 "./include.c" 2

int main()
{
    foo();
    return 0;
}
