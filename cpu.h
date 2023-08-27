#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <string.h>

#include "assert.h"

struct memory_region {
	uint32_t size;
	uint32_t offset;
	uint8_t *ptr;
};

#ifndef NUM_REGISTERS
#define NUM_REGISTERS 16
#endif

typedef struct {
    uint64_t cycles;
    uint32_t pc;
    uint32_t registers[NUM_REGISTERS];

    struct memory_region ROM;
    struct memory_region RAM;
    struct memory_region IN;
    struct memory_region OUT;

    uint8_t *memory;
} Cpu;

extern char* register_name[];

inline
static
int register_by_name(char *name) {
	for(int i = 0; i < NUM_REGISTERS; i++) {
		if (strcmp(name, register_name[i]) == 0) return i;
	}
	_assert(0);
	return 0;
}

void initCpu(Cpu* cpu);

void syncCpu(Cpu* cpu);

int dumpRegister(Cpu* cpu, char* buf);

uint32_t fetch(Cpu* cpu);

#endif
