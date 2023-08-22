unsigned long ecall(unsigned long which, unsigned long arg0, unsigned long arg1, unsigned long arg2) {
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

unsigned long ecall_0(unsigned long which) {
	return ecall(which, 0, 0, 0);
}

void main(void)
{
    ecall(0, 'H', 0, 0);
    ecall(0, 'e', 0, 0);
    ecall(0, 'l', 0, 0);
    ecall(0, 'l', 0, 0);
    ecall(0, 'o', 0, 0);
    ecall(0, ' ', 0, 0);

    ecall(1, (unsigned long) "World!", 6, 0);
}
