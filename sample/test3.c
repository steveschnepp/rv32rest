#define SIZE 64

// pi = 4 · [ 1 – 1/3 + 1/5 – 1/7 + 1/9 ... + (–1)^n/(2n + 1) ]

volatile double b = 0;

void main() {
	int s = 1;
	for (int a = 1; a <= 100000; a++) {
		s = -s;
		b += 4 * (s/ (2 * a + 1));
	}
}
