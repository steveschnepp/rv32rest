#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#include "cpu.h"

void dumpMemory(Cpu* cpu) {
    for (int i = 0; i < MEM_SIZE_32; i++) {
        printf("0x%04lx : 0x%08x\n", i * sizeof(uint32_t), cpu->memory[i]);
    }
    return;
}

void dumpRegister(Cpu* cpu) {
    for (int i = 0; i < 16; i++) {
        printf("%-5s: 0x%08x", register_name[i], cpu->registers[i]);
        printf("\t");
        printf("%-5s: 0x%08x", register_name[i+16], cpu->registers[i+16]);
        printf("\n");
    }
    printf("pc   : 0x%08x\n", cpu->pc);
    return;
}

#endif
