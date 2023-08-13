#include "printf.h"

unsigned long long read_cycles() {
	unsigned long high = 0;
	unsigned long low = 0;

    asm volatile ("rdcycle %0"
              : "+r" (low)
              :
              : "memory");
    asm volatile ("rdcycleh %0"
              : "+r" (high)
              :
              : "memory");
    unsigned long long cycles = ((unsigned long long) high << 32) + low;
    return cycles;
}

void main(void)
{
	for (int i = 0; i < 10; i ++) {
		unsigned long long cycles = read_cycles();
		printf("%llu ", cycles);
	}

}
