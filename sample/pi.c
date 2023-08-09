#include "printf.h"

// pi = 4 · [ 1 – 1/3 + 1/5 – 1/7 + 1/9 ... + (–1)^n/(2n + 1) ]

static double b = 0;

void main() {
	int s = 1;
	for (int a = 1; a <= 10; a++) {
		s = -s;
		b += s / (2.0 * a + 1);
		printf("%d %f ", a, b);
	}
}
