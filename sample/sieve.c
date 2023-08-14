#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "csr.h"
#include "printf.h"

#define true 1
#define false 0

#define BITSET_SIZE (1024)
#include "bitset.h"

#define max 1024

int main() {
    int i, prime, k, count, iter;
    printf("%d iterations\n", max);
        unsigned long long c = read_cycles();
        unsigned long long t = read_times();
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
	printf("%d primes\n", count);
	unsigned long long c_diff = read_cycles() - c;
	unsigned long long t_us = read_times() - t;

        printf("%llu cycles in ", read_cycles() - c);
        printf("t:%dms ", t_us / 1000ULL);
    return count;
}
