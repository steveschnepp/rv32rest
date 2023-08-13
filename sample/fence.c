void main() {
	asm volatile("": : :"memory");
	asm("fence r, w");
	asm("fence i, o");
	asm("fence i, i");
	asm("nop");
	__sync_synchronize();
}
