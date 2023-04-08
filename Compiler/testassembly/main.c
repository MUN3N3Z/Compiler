#include <stdio.h>
// to run on M1 Mac : arch -x86_64 gcc main.c assembly.s
extern long foo(long a, long b);
int main()
{
    long a = 12, b = 6, c;
    c = foo(a,b);
    printf("\n result is : %ld", c);
    return 0;
}
