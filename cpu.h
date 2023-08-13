#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>

struct memory_region {
	uint32_t size;
	uint32_t offset;
	uint8_t *ptr;
};

#ifndef NUM_REGISTERS
#define NUM_REGISTERS 16
#endif

typedef struct {
    uint32_t pc;
    uint32_t registers[NUM_REGISTERS];

    struct memory_region ROM;
    struct memory_region RAM;
    struct memory_region IN;
    struct memory_region OUT;

    uint8_t *memory;
} Cpu;

extern char* register_name[];

void initCpu(Cpu* cpu);

void syncCpu(Cpu* cpu);

int dumpRegister(Cpu* cpu, char* buf);

uint32_t fetch(Cpu* cpu);

#endif
