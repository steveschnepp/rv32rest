
#include <stdio.h>
#include <string.h>

#define OUT ((void *) 0x08100000)
long out_offset = 0;

int _write__(int fd, char* b, int size) {
    char *out = OUT+4+out_offset;
    memcpy(out, b, size);
    out_offset += size;
}

static void
outbyte (char  c)
{
}       /* outbyte () */

static int
sample_putc(char c, FILE *file)
{
	(void) file;		/* Not used in this function */
    char *out = OUT+4+out_offset;
    *out=c;
    out_offset ++;
	return c;
}

static int
sample_getc(FILE *file)
{
	return EOF;
}

static FILE __stdio = FDEV_SETUP_STREAM(sample_putc,
					sample_getc,
					NULL,
					_FDEV_SETUP_RW);

FILE *const __iob[3] = { &__stdio, &__stdio, &__stdio };

void main() {
	printf("Hello World!\n");
}
