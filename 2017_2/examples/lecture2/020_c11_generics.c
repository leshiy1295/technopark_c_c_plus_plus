#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define typename(x) _Generic((x),        /* Get the name of a type */             \
                                                                                  \
        _Bool: "_Bool",                  unsigned char: "unsigned char",          \
         char: "char",                     signed char: "signed char",            \
    short int: "short int",         unsigned short int: "unsigned short int",     \
          int: "int",                     unsigned int: "unsigned int",           \
     long int: "long int",           unsigned long int: "unsigned long int",      \
long long int: "long long int", unsigned long long int: "unsigned long long int", \
        float: "float",                         double: "double",                 \
  long double: "long double",                   char *: "pointer to char",        \
       void *: "pointer to void",                int *: "pointer to int",         \
      default: "other")

#define fmt "%20s is '%s'\n"
int main() {

  size_t s; ptrdiff_t p; intmax_t i; int ai[3] = {0};
  printf( fmt fmt fmt fmt fmt fmt fmt fmt,

     "size_t", typename(s),               "ptrdiff_t", typename(p),     
   "intmax_t", typename(i),      "character constant", typename('0'),
 "0x7FFFFFFF", typename(0x7FFFFFFF),     "0xFFFFFFFF", typename(0xFFFFFFFF),
"0x7FFFFFFFU", typename(0x7FFFFFFFU),  "array of int", typename(ai));
  return 0;
}
