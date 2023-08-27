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

__attribute__((weak))
void main(void)
{
    ecall_3(0, 'H', 0, 0);
    ecall_1(0, 'e');
    ecall_1(0, 'l');
    ecall_1(0, 'l');
    ecall_1(0, 'o');
    ecall_1(0, ' ');

    ecall_2(1, (unsigned long) "World!", 6);
}
