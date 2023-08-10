#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "printf.h"

#define true 1
#define false 0

#define BITSET_SIZE (64 * 1024)
#include "bitset.h"

#define max 1000

int main() {
    int i, prime, k, count, iter;
    printf("%d iterations\n", max);
    for (iter = 1; iter <= max; iter ++) {
        count=0;
        memset(bitfields, 0, sizeof(bitfields));
        for (i = 0; i < BITSET_SIZE; i++) {
            if (!getbit(i)) {
                prime = i * 2 + 3;
                k = i + prime;
                while (k <= BITSET_SIZE) {
                    setbit(i, 1);
                    k += prime;
                }
                count = count + 1;
            }
        }
    }
    printf("%d primes", count);
    return count;
}
