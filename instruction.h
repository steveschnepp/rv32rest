#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "cpu.h"

void execution(Cpu* cpu, uint32_t instruction);

#endif
