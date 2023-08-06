#include <stdio.h>

#define OUT ((void *) 0x08100000)
long out_offset = 0;

// first two values
long prev1 = 1;
long prev2 = 0;

// recursive function to print the fibonacci series
void fib(int n)
{
    if (n < 3) {
        return;
    }
    long fn = prev1 + prev2;
    prev2 = prev1;
    prev1 = fn;
    char *out = OUT+4+out_offset;
    out_offset += sprintf(out, "%d ", fn);
    return fib(n - 1);
}

// function that handles the first two terms and calls the
// recursive function
void printFib(int n)
{
    // when the number of terms is less than 1
    if (n < 1) {
    char *out = OUT+4+out_offset;
        out_offset += sprintf(out, "Invalid number of terms\n");
    }
    // when the number of terms is 1
    else if (n == 1) {
    char *out = OUT+4+out_offset;
        out_offset += sprintf(out, "%d ", 0);
    }
    // when the number of terms is 2
    else if (n == 2) {
    char *out = OUT+4+out_offset;
        out_offset += sprintf(out, "%d %d", 0, 1);
    }
    // number of terms greater than 2
    else {
    char *out = OUT+4+out_offset;
        out_offset += sprintf(out, "%d %d ", 0, 1);
        fib(n);
    }
    return;
}

// driver code
int main()
{
    int n = 1024;
    printFib(n);
    return 0;
}
