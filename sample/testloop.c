volatile char   a = 0xEE;
volatile short  b;
volatile int    c;

void main() {
    while (1) {
        a ++;
	if (a % 7) b++;
	if (b % 9) c++;
    }
}
