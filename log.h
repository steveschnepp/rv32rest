#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#include "cpu.h"

void dumpMemory(Cpu* cpu) {
    for (int i = 0; i < RAM_SIZE_32; i++) {
        printf("0x%04x : 0x%08x\n", i * 4, cpu->memory[i]);
    }
    return;
}

void dumpRegister(Cpu* cpu) {
    for (int i = 0; i < 32; i++) {
        printf("reg[%02d]: 0x%08x\n", i, cpu->registers[i]);
    }
    printf("PC     : 0x%08x\n", cpu->pc);
    return;
}

#endif
