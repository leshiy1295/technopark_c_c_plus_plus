extern void foo(int);

#define FOO()   foo()
#define BAR()   FOO()
#define BAZ()   BAR()

int main(void)
{
    BAZ();
    return 0;
}
