#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <string.h>

struct memory_region {
	uint32_t size;
	uint32_t offset;
	uint8_t *ptr;
};

typedef struct {
    uint32_t pc;
    uint32_t registers[32];

    struct memory_region ROM;
    struct memory_region RAM;
    struct memory_region IN;
    struct memory_region OUT;

    uint32_t *memory;
} Cpu;

char* register_name[] = {"zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                         "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                         "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                         "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

void initCpu(Cpu* cpu) {
    cpu->pc = 0x0000;
    memset(cpu->registers, 0, sizeof(cpu->registers));

    // Init stack pointer to end of MEM_SIZE
    cpu->registers[2] = cpu->RAM.offset + cpu->RAM.size;
    return;
}

uint32_t fetch(Cpu* cpu) { return cpu->memory[cpu->pc / 4]; }

#endif
