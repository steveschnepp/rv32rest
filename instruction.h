#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "cpu.h"

#ifdef DEBUG
#define trace printf
#else
#define trace _trace
#endif

#ifdef DEBUG_LOAD
#define trace_load printf
#else
#define trace_load _trace_load
#endif

#ifdef DEBUG_STORE
#define trace_store printf
#else
#define trace_store _trace_store
#endif

#ifndef sign_expansion
#define sign_expansion sign_expansion_for
#endif

void execution(Cpu* cpu, uint32_t instruction);

#endif
