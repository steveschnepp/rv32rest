#include <stdio.h>

#include "printf.h"

typedef unsigned long fibo_t;

static
fibo_t fibo(fibo_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    fibo_t fn1 = 1;
    fibo_t fn2 = 1;
    fibo_t fn = 0;
    for (fibo_t i = 1; i < n; i ++) {
        fn = fn1 + fn2;
        fn2 = fn1;
        fn1 = fn; 
    }

    return fn;
}

void printFib(int n)
{
    for (fibo_t i = 1; i < n; i ++) {
        fibo_t f = fibo(i);
        printf("%d ", i);
    }
}

// driver code
int main()
{
    int n = 32;
    printFib(n);
    return 0;
}
