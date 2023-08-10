#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <string.h>

struct memory_region {
	uint32_t size;
	uint32_t offset;
	uint8_t *ptr;
};

#define NUM_REGISTERS 16

typedef struct {
    uint32_t pc;
    uint32_t registers[NUM_REGISTERS];

    struct memory_region ROM;
    struct memory_region RAM;
    struct memory_region IN;
    struct memory_region OUT;

    uint8_t *memory;
} Cpu;

char* register_name[] = {"zero", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                         "s0",   "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                         "a6",   "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                         "s8",   "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

void initCpu(Cpu* cpu) {
    cpu->pc = 0x0000;
    memset(cpu->registers, 0, sizeof(cpu->registers));

    // Init stack pointer to end of MEM_SIZE
    cpu->registers[2] = cpu->RAM.offset + cpu->RAM.size - sizeof(Cpu);
    return;
}

void syncCpu(Cpu* cpu) {
	void* end_memory = cpu->memory + cpu->RAM.offset + cpu->RAM.size;
	end_memory -= sizeof(cpu->registers);
	memcpy(end_memory, cpu->registers, sizeof(cpu->registers));
	end_memory -= sizeof(uint32_t);
	memcpy(end_memory, &cpu->pc, sizeof(uint32_t));
	end_memory -= 4;
	memcpy(end_memory, "CPU ", 4);
}

uint32_t fetch(Cpu* cpu) {
    uint32_t* instructions = (uint32_t*) cpu->memory;
    return instructions[cpu->pc / 4];
}

#endif
