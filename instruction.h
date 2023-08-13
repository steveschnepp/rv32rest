#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "cpu.h"

void execution(Cpu* cpu, uint32_t instruction);

// Those are declared weak and can be overridden
void unsupported_opcode(Cpu* cpu, uint32_t instruction);
uint64_t times();

void ecall_callback(Cpu* cpu);
void ebreak_callback(Cpu* cpu);

#endif
