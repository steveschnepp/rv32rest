#include "ecall.h"

unsigned long ecall_3(unsigned long which, unsigned long arg0, unsigned long arg1, unsigned long arg2) {
    register unsigned long a0 asm ("a0") = (unsigned long)(arg0);
    register unsigned long a1 asm ("a1") = (unsigned long)(arg1);
    register unsigned long a2 asm ("a2") = (unsigned long)(arg2);
    register unsigned long t2 asm ("t2") = (unsigned long)(which);
    asm volatile ("ecall"
              : "+r" (a0)
              : "r" (a1), "r" (a2), "r" (t2)
              : "memory");
    return a0;
}

unsigned long ecall_2(unsigned long which, unsigned long arg0, unsigned long arg1) {
    register unsigned long a0 asm ("a0") = (unsigned long)(arg0);
    register unsigned long a1 asm ("a1") = (unsigned long)(arg1);
    register unsigned long t2 asm ("t2") = (unsigned long)(which);
    asm volatile ("ecall"
              : "+r" (a0)
              : "r" (a1), "r" (t2)
              : "memory");
    return a0;
}

unsigned long ecall_1(unsigned long which, unsigned long arg0) {
    register unsigned long a0 asm ("a0") = (unsigned long)(arg0);
    register unsigned long t2 asm ("t2") = (unsigned long)(which);
    asm volatile ("ecall"
              : "+r" (a0)
              :  "r" (t2)
              : "memory");
    return a0;
}

unsigned long ecall_0(unsigned long which) {
    register unsigned long a0 asm ("a0");   
    register unsigned long t2 asm ("t2") = (unsigned long)(which);
    asm volatile ("ecall"
              : "+r" (a0)
              : "r" (t2)
              : "memory");
    return a0;
}

void ebreak() {
    asm volatile ("ebreak"
              :
              :
              : "memory");
}

__attribute__((weak))
void main(void)
{
    ecall_3(SYS_PUTC, 'H', 0, 0);
    ecall_1(SYS_PUTC, 'e');
    ecall_1(SYS_PUTC, 'l');
    ecall_1(SYS_PUTC, 'l');
    ecall_1(SYS_PUTC, 'o');
    ecall_1(SYS_PUTC, ' ');

    ecall_2(SYS_PUTS, (unsigned long) "World!\n", 6);

    ebreak();
}
