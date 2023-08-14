#include "csr.h"
#include "printf.h"


void main(void)
{
	for (int i = 0; i < 10; i ++) {
		unsigned long long c = read_cycles();
		printf("c:%llu ", c);
		unsigned long long t = read_times();
		printf("t:%llu ", t);
	}

}
