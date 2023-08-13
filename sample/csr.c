#include "printf.h"

unsigned long read_cycles() {
    register unsigned long a0 asm ("a0") = 0;
    asm volatile ("rdcycle a0"
              : "+r" (a0)
              :
              : "memory");
    return a0;
}

void main(void)
{
	for (int i = 0; i < 10; i ++) {
		unsigned long cycles = read_cycles();
		printf("%u ", cycles);
	}

}
