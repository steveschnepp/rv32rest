#include <stdlib.h>

void _exit (int status) 
{
    asm("j 0"); // no return
    __builtin_unreachable();
} 


int main() {
	exit(0);
}
