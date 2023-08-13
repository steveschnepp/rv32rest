void main() {
	asm volatile("": : :"memory");
	asm("fence r, w");
	asm("fence i, o");
	asm("fence i, i");
	__sync_synchronize();
}
