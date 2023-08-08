#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>


#include "cpu.h"
#include "instruction.h"
#include "log.h"

int open2prot(int mode) {
	switch(mode) {
		default:
		case O_RDONLY: return PROT_READ;
		case O_WRONLY: return PROT_WRITE;
		//case O_WRONLY:
		case O_RDWR:   return PROT_READ | PROT_WRITE;
	}
}

void handle_error(char *what) {
	perror(what);
	exit(EXIT_FAILURE);
}

/* get a 4G contiguous mapping */
void *get4g() {
	void *ptr = mmap(NULL, 0xFFFFFFFF, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
	if (ptr == MAP_FAILED) handle_error("get4g/mmap");

	munmap(ptr, 0xFFFFFFFF);
	return ptr;
}


struct memory_region mmap_file(char *file, int mode, uint8_t *memory, uint32_t offset) {
	int fd = open(file, mode);
	if (fd == -1) handle_error("mmap_file/open");

	/* To obtain file size */
	struct stat sb;
	if (fstat(fd, &sb) == -1) handle_error("mmap_file/fstat");
	size_t len = sb.st_size;

	struct memory_region r = {0};
	r.size = len;
	r.offset = offset;
	r.ptr = mmap(memory + offset, len, open2prot(mode), MAP_SHARED, fd, 0);
	assert(r.ptr == memory + r.offset);

	return r;
}

int main(int argc, char** argv) {
	argc --; argv++; // Ignore the program name

	if (argc != 4) {
		printf("usage: rv32im <rom> <ram> <stdin> <stdout>\n");
		return 1;
	}

	Cpu cpu;

	cpu.memory = get4g();

	cpu.ROM = mmap_file(*argv, O_RDONLY, cpu.memory, 0x00000000); argv++;
	cpu.RAM = mmap_file(*argv, O_RDWR,   cpu.memory, 0x01000000); argv++;
	cpu.IN  = mmap_file(*argv, O_RDONLY, cpu.memory, 0x08000000); argv++;
	cpu.OUT = mmap_file(*argv, O_RDWR,   cpu.memory, 0x08100000); argv++;

	initCpu(&cpu);

	// OUT is always 0-init
	memset(cpu.OUT.ptr, 0, cpu.OUT.size);

	while(1) {
		static uint64_t loop_cnt = 0;
		uint32_t instruction = fetch(&cpu);
		trace("% 6ld %4x: %08x\t\t", loop_cnt, cpu.pc, instruction);
		execution(&cpu, instruction);
		loop_cnt++;

#if 0
		if (__builtin_expect(loop_cnt, 1) > 0x0100000000) {
			// Stop when looping too many times
			printf("Timeout.\n");
			break;
		}
#endif
		if (!__builtin_expect(cpu.pc, 1)) {
			// End when the program counter returns to 0
			printf("Finish in %lx cycles.\n", loop_cnt);
			break;
		}

//		usleep(10 * 1000);
	}

	return 0;
}
