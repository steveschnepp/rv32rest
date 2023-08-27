
#include <stdio.h>
#include <string.h>

#ifdef __RV32REST__

#define OUT 0x08800000
#define OUT_BUFFER ((void *) OUT+4)
#define OUT_OFFSET (*((unsigned int*) OUT))

#ifdef PRINTF_ECALL
#include "ecall.h"
static int
sample_putc(char c, FILE *file)
{
    (void) file;		/* Not used in this function */
    ecall_1(0, c);
    return c;
}

static int
sample_getc(FILE *file)
{
	return EOF;
}
#else
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
#endif

static FILE __stdio = FDEV_SETUP_STREAM(sample_putc,
					sample_getc,
					NULL,
					_FDEV_SETUP_RW);

static FILE *const __iob[3] = { &__stdio, &__stdio, &__stdio };

#if _PICOLIBC_MINOR__ >= 8
FILE *const stdin = __iob[0];
FILE *const stdout = __iob[1];
FILE *const stderr = __iob[2];
#endif

#if _PICOLIBC_MINOR__ >= 7
FILE *const stdin = __iob[0];
FILE *const stdout = __iob[1];
FILE *const stderr = __iob[2];
#endif

#endif
