#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "cpu.h"
#include "instruction.h"
#include "log.h"
#include "assert.h"

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

/* get a virtual mapping of contiguous memory */
void *reserve_memory(uint32_t size) {
	void *ptr = mmap(NULL, size, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
	if (ptr == MAP_FAILED) handle_error("reserve_memory/mmap");

	munmap(ptr, size);
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
	_assert(r.ptr == memory + r.offset);

	return r;
}

static
int max_loop(uint64_t loop_cnt) {
#ifdef MAX_LOOPS
	if (__builtin_expect(loop_cnt, 1) > MAX_LOOPS) {
			return 1;
	}
#endif
	return 0;
}

static
void yield() {
#ifdef SLEEP_MS
	usleep(SLEEP_MS * 1000);
#endif
}

int main(int argc, char** argv) {
	argc --; argv++; // Ignore the program name

	if (argc != 4) {
		printf("usage: rv32im <rom> <ram> <stdin> <stdout>\n");
		return 1;
	}

	Cpu cpu;

	// allocate 1G
	cpu.memory = reserve_memory(0x10000000);

	// Everything has to fit into 0x00000000 - 0x10000000
	cpu.ROM = mmap_file(*argv, O_RDONLY, cpu.memory, 0x00000000); argv++;
	cpu.RAM = mmap_file(*argv, O_RDWR,   cpu.memory, 0x01000000); argv++;
	cpu.IN  = mmap_file(*argv, O_RDONLY, cpu.memory, 0x08000000); argv++;
	cpu.OUT = mmap_file(*argv, O_RDWR,   cpu.memory, 0x08800000); argv++;

	initCpu(&cpu);

	// OUT is always 0-init
	memset(cpu.OUT.ptr, 0, cpu.OUT.size);

	static uint64_t loop_cnt = 0;
	do {
		int32_t instruction = fetch(&cpu);
		trace("%10" PRIu64 " %4x: %08x\t\t", loop_cnt, cpu.pc, instruction);
		execution(&cpu, instruction);
		loop_cnt++;

		if (max_loop(loop_cnt)) {
			// Stop when looping too many times
			printf("Timeout.\n");
			break;
		}

		yield();

//		usleep(10 * 1000);

	} while (__builtin_expect(cpu.pc, 1));

	// End when the program counter returns to 0
	printf("Finish in %" PRIx64 " cycles.\n", loop_cnt);

	return 0;
}
