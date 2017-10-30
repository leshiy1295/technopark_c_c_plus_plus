#include <stdio.h>

int main(int argc, char *argv[]) {
  int a = 0;
  int b = 5;
  int i;
  i = (a += 2, a + b);
  printf("%d", i);
  return 0;
}
