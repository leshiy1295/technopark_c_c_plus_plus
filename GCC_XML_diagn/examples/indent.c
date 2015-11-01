extern int printf(const char *, ...);

int main(int argc, char **argv)
{
    (void)argv;

    if (argc == 1)
        printf("No arguments!\n");
    else
        printf("The program was invoked with some arguments.\n");
        printf("%d arguments passed.\n", argc - 1);
    return 0;
}
