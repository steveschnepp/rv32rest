#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#include "cpu.h"

void dumpRegister(Cpu* cpu) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("%-5s: 0x%08x", register_name[i], cpu->registers[i]);
#if NUM_REGISTERS > 16
        printf("\t");
        printf("%-5s: 0x%08x", register_name[i+16], cpu->registers[i+16]);
#endif // NUM_REGISTERS > 16
        printf("\n");
    }
    printf("pc   : 0x%08x\n", cpu->pc);
    return;
}

#endif
