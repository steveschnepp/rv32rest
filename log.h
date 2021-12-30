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
        printf("%-5s: 0x%08x\n", register_name[i], cpu->registers[i]);
    }
    printf("pc   : 0x%08x\n", cpu->pc);
    return;
}

#endif
