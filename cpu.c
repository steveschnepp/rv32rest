#include "cpu.h"

#include <string.h>
#include <stdio.h>

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

void syncCpu(Cpu* cpu) {
	void* end_memory = cpu->memory + cpu->OUT.offset + cpu->OUT.size;
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

int dumpRegister(Cpu* cpu, char *buf) {
	if (! buf) return 0;
	char *b = buf;
	memset(b, 0, 1024);
    for (int i = 0; i < NUM_REGISTERS && (NUM_REGISTERS > 16 && i+16 < NUM_REGISTERS ); i++) {
        b += sprintf(b, "%-5s: 0x%08x", register_name[i], cpu->registers[i]);
#if NUM_REGISTERS > 16
        b += sprintf(b, "\t");
        b += sprintf(b, "%-5s: 0x%08x", register_name[i+16], cpu->registers[i+16]);
#endif // NUM_REGISTERS > 16
        b += sprintf(b, "\n");
    }
    b += sprintf(b, "pc   : 0x%08x\n", cpu->pc);

    return b - buf;
}
