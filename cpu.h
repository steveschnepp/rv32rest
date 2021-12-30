#ifndef CPU_H_
#define CPU_H_

#include <stdint.h>
#include <string.h>

#define ROM_SIZE 1024
#define RAM_SIZE 1024
#define MEM_SIZE ROM_SIZE + RAM_SIZE
#define ROM_SIZE_32 (ROM_SIZE / sizeof(uint32_t))
#define RAM_SIZE_32 (RAM_SIZE / sizeof(uint32_t))
#define MEM_SIZE_32 (MEM_SIZE / sizeof(uint32_t))

typedef struct {
    uint32_t pc;
    uint32_t registers[32];
    uint32_t memory[MEM_SIZE_32];
} Cpu;

void initCpu(Cpu* cpu) {
    cpu->pc = 0x0000;
    memset(cpu->registers, 0, sizeof(cpu->registers));
    memset(cpu->memory, 0, sizeof(cpu->memory));
    return;
}

uint32_t fetch(Cpu* cpu) { return cpu->memory[cpu->pc / 4]; }

#endif
