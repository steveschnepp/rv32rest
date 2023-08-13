
#include <stdio.h>
#include <string.h>

#ifndef __GNU_LIBRARY__

#define OUT 0x08800000
#define OUT_BUFFER ((void *) OUT+4)
#define OUT_OFFSET (*((unsigned int*) OUT))

static int
sample_putc(char c, FILE *file)
{
    (void) file;		/* Not used in this function */
    char *out = OUT_BUFFER + OUT_OFFSET;
    *out=c;
    OUT_OFFSET++;
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

#endif
