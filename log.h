#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#include "cpu.h"

#ifdef DEBUG
#define trace printf
#else
inline static
void trace(char *c, ...) { }
#endif

#ifdef DEBUG_LOAD
#define trace_load printf
#else
inline static
void trace_load(char *c, ...) { }
#endif

#ifdef DEBUG_STORE
#define trace_store printf
#else
inline static
void trace_store(char *c, ...) { }
#endif

#endif
