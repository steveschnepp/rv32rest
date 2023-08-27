#ifndef __ECALL_H__
#define __ECALL_H__

enum SYSCALLS {
    SYS_PUTC = 0,
    SYS_PUTS = 1,
};

unsigned long ecall_3(unsigned long which, unsigned long arg0, unsigned long arg1, unsigned long arg2);
unsigned long ecall_2(unsigned long which, unsigned long arg0, unsigned long arg1);
unsigned long ecall_1(unsigned long which, unsigned long arg0);
unsigned long ecall_0(unsigned long which);

void ebreak();

#endif // __ECALL_H__
