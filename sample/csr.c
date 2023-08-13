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

unsigned long long read_times() {
	unsigned long high = 0;
	unsigned long low = 0;

    asm volatile ("rdtime %0"
              : "+r" (low)
              :
              : "memory");
    asm volatile ("rdtimeh %0"
              : "+r" (high)
              :
              : "memory");
    unsigned long long times = ((unsigned long long) high << 32) + low;
    return times;
}

void main(void)
{
	for (int i = 0; i < 10; i ++) {
		unsigned long long c = read_cycles();
		printf("c:%llu ", c);
		unsigned long long t = read_times();
		printf("t:%llu ", t);
	}

}
