#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define OUT ((void *) 0x08100000)
long out_offset = 0;

#define true 1
#define false 0
#define size (64 * 1024)

#define bool _Bool

#ifdef BIT_COMPRESS
uint32_t bitfields[size / sizeof(uint32_t) / 8];
void setbit(uint32_t bit, bool value)
{
	const int s = sizeof(*bitfields);
	int off = bit / s;
	int shift = bit % s;
	if (value)
		bitfields[off] |= (1 << shift);
	else
		bitfields[off] &= ~(1 << shift);
}
bool getbit(uint32_t bit)
{
	const int s = sizeof(*bitfields);
	int off = bit / s;
	int shift = bit % s;
	bool value = bitfields[off] & (1 << shift);
	return value;
}
#else
bool bitfields[size];
void setbit(uint32_t bit, bool value)
{
	bitfields[bit] = value;
}
bool getbit(uint32_t bit)
{
	return bitfields[bit];
}

#endif


#define max 1000

int main() {
    int i, prime, k, count, iter;
    {
    char *out = OUT+4+out_offset;
    out_offset += sprintf(out, "%d iterations\n", max);
    }
    for (iter = 1; iter <= max; iter ++) {
        count=0;
        memset(bitfields, 0, sizeof(bitfields));
        for (i = 0; i < size; i++) {
            if (!getbit(i)) {
                prime = i * 2 + 3;
                k = i + prime;
                while (k <= size) {
                    setbit(i, 1);
                    k += prime;
                }
                count = count + 1;
            }
        }
    }
    {
    char *out = OUT+4+out_offset;
    out_offset += sprintf(out, "%d primes", count);
    }
    return count;
}
