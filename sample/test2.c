#define SIZE ('Z'-'A')

volatile char   a[SIZE];
volatile short  b[SIZE];
volatile int    c[SIZE];

volatile char   d[SIZE];
volatile short  e[SIZE];
volatile int    f[SIZE];

void main() {
    for (int i = 0; i < SIZE; i++) {
        a[i] = i + 'A';
        b[i] = i + 0xDB00 + 'B';
        c[i] = i + 0xBEEF0000 + 'C';
    }


    for (int i = 0; i < SIZE; i++) {
        d[i] = a[i] + 1;
        e[i] = b[i] + 1;
        f[i] = c[i] + 1;
    }
}
