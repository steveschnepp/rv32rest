void main() {
	asm volatile("": : :"memory");
	asm("fence r, w"); // syntax is "fence predecessor[iorw], successor[iorw]"
	asm("fence i, o");
	asm("fence i, rw");
	asm("nop");
	__sync_synchronize();
}
