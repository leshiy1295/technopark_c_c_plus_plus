#ifndef HEADER2_H
#define HEADER2_H

extern char *a;
extern char *b;

static inline
char *foo()
{
    return a + b;
}

#endif
