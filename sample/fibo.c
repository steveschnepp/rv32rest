#include <stdio.h>

#include "printf.h"

typedef unsigned int fibo_t;

// recursive function to print the fibonacci series
static
fibo_t fib(fibo_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    fibo_t fn1 = 1;
    fibo_t fn2 = 1;
    fibo_t fn = 0;
    for (int i = 2; i < n; i ++) {
        fn = fn1 + fn2;
        fn2 = fn1;
        fn1 = fn;
    }

    return fn;
}

// function that handles the first two terms and calls the
// recursive function
void printFib(int n)
{
    // when the number of terms is less than 1
    if (n < 1) {
        printf("Invalid number of terms\n");
    }
    // when the number of terms is 1
    else if (n == 1) {
    printf("%d ", 0);
    }
    // when the number of terms is 2
    else if (n == 2) {
    printf("%d %d", 0, 1);
    }
    // number of terms greater than 2
    else {
        printf("%d %d ", 0, 1);
        fib(n);
    }
    return;
}

// driver code
int main()
{
    int n = 32;
    printFib(n);
    return 0;
}
