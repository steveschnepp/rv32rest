#include "printf.h"

// pi = 4 · [ 1 – 1/3 + 1/5 – 1/7 + 1/9 ... + (–1)^n/(2n + 1) ]

void main() {
	long double b = 1;
	double s = 1;
	for (long long a = 3; a < 1024 * 2; a += 2) {
		s = -s;
		b += s / (double) a;
		double pi = b * 4;
		if(!((a-3) % 16)) printf("%lld %.12f\n", a, pi);
	}
}
