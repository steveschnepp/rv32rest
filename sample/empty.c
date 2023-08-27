#include <stdlib.h>

void _exit (int status) 
{
    asm("j 0"); // no return
} 


int main() {
//    exit(0);
}
