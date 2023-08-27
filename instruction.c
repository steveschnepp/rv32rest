#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "assert.h"

#include "instruction.h"

// Code expansion
static inline
int32_t sign_expansion_sw(uint32_t imm, uint8_t width) {
    int32_t s;
	switch(width) {
	    default:
		    _assert(0);
	    case 12:
		    if (imm & (1<<11)) imm |= ~0xFFF;
		    break;
	    case 13:
		    if (imm & (1<<12)) imm |= ~0x1FFF;
		    break;
	    case 16:
		    if (imm & (1<<15)) imm |= ~0xFFFF;
		    break;
	    case 20:
		    if (imm & (1<<19)) imm |= ~0xFFFFF;
		    break;
	}
	s = (int32_t) imm;
	return s;
}

static inline
int32_t sign_expansion_for(uint32_t imm, uint8_t width) {
    uint32_t mask = 0xFFFFFFFF;
    if (imm >> (width - 1)) {
        for (uint8_t i = 0; i < width; i++) {
            mask -= (0x1 << i);
        }
        return (imm | mask);
    } else {
        return imm;
    }
}

static inline
int32_t sign_expansion(uint32_t imm, uint8_t width) {
#ifdef SIGN_EXPANSION_FOR
    return sign_expansion_for(imm, width);
#else
    return sign_expansion_sw(imm, width);
#endif
}

static
void set_pc(Cpu* cpu, uint32_t pc) {
    cpu->pc = pc;
}

static
void increment_pc(Cpu* cpu) {

    set_pc(cpu, cpu->pc + 4);
}

static
void store_rd(Cpu* cpu, uint8_t rd, uint32_t value) {
    _assert(rd < NUM_REGISTERS);
    if (rd) {
        cpu->registers[rd] = value;
    }
}

static
void lui(Cpu* cpu, uint32_t imm, uint8_t rd) {
    uint32_t value = imm << 12;
    trace("lui\t%s, 0x%x \t# %08x\n", register_name[rd], imm, value);
    store_rd(cpu, rd, value);
    increment_pc(cpu);
    return;
}

static
void auipc(Cpu* cpu, uint32_t imm, uint8_t rd) {
    uint32_t value = cpu->pc + (imm << 12);
    trace("auipc\t%s, 0x%x \t# %x\n", register_name[rd], imm, value);
    store_rd(cpu, rd, value);
    increment_pc(cpu);
    return;
}

static
void jal(Cpu* cpu, uint32_t imm, uint8_t rd) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 20);
    trace("jal\t%s, %x  \t# @%x\n", register_name[rd], sign_expansion(imm, 20), addr);
    store_rd(cpu, rd, cpu->pc + 4);
    set_pc(cpu, addr);
    return;
}

static
void jalr(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    trace("jalr\t%s, %s, %x \t# @%x\n", register_name[rd], register_name[rs1], sign_expansion(imm, 12), addr);
    store_rd(cpu, rd, cpu->pc + 4);
    set_pc(cpu, addr);
    return;
}

static
void beq(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint8_t value = op1 == op2;
    if (value) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    trace("beq\t%s, %s, %d \t# @%x %s\n", register_name[rs1], register_name[rs2], sign_expansion(imm, 13), addr, value ? "taken" : "");
    return;
}

static
void bne(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint8_t value = op1 != op2;
    if (value) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    trace("bne\t%s, %s, %d \t# @%x %s\n", register_name[rs1], register_name[rs2], sign_expansion(imm, 13), addr, value ? "taken" : "");
    return;
}

static
void blt(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    uint8_t value = op1 < op2;
    if (value) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    trace("blt\t%s, %s, %d \t# @%x %s\n", register_name[rs1], register_name[rs2], sign_expansion(imm, 13), addr, value ? "taken" : "");
    return;
}

static
void bge(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    uint8_t value = op1 >= op2;
    if (value) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    trace("bge\t%s, %s, %d \t# @%x %s\n", register_name[rs1], register_name[rs2], sign_expansion(imm, 13), addr, value ? "taken" : "");
    return;
}

static
void bltu(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint8_t value = op1 < op2;
    if (value) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    trace("bltu\t%s, %s, %d \t# @%x %s\n", register_name[rs1], register_name[rs2], sign_expansion(imm, 13), addr, value ? "taken" : "");
    return;
}

static
void bgeu(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    uint8_t value = op1 >= op2;
    if (value) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    trace("bgeu\t%s, %s, %d \t# @%x %s\n", register_name[rs1], register_name[rs2], sign_expansion(imm, 13), addr, value ? "taken" : "");
    return;
}

static
void store4(uint32_t ofs, uint32_t val, uint8_t *memory) {
	trace_store("s4u(%04x)=%08x\n", ofs, val);
	*(uint32_t*)(memory + ofs) = val;
}
static
void store2(uint32_t ofs, uint16_t val, uint8_t *memory) {
	trace_store("s2u(%04x)=%08x\n", ofs, val);
	*(uint16_t*)(memory + ofs) = val;
}
static
void store1(uint32_t ofs, uint8_t val, uint8_t *memory) {
	trace_store("s1u(%04x)=%08x\n", ofs, val);
	*(uint8_t*)(memory + ofs) = val;
}

uint32_t load4(uint32_t ofs, uint8_t *memory) {
	uint32_t val = *(uint32_t*)(memory + ofs);
	trace_load("l4u(%04x)=%08x\n", ofs, val);
	return val;
}
uint16_t load2(uint32_t ofs, uint8_t *memory) {
	uint16_t val = *(uint16_t*)(memory + ofs);
	trace_load("l2u(%04x)=%08x\n", ofs, val);
	return val;
}
uint8_t load1(uint32_t ofs, uint8_t *memory) {
	uint8_t val = *(uint8_t*)(memory + ofs);
	trace_load("l1u(%04x)=%08x\n", ofs, val);
	return val;
}

int16_t load2s(uint32_t ofs, uint8_t *memory) {
	uint16_t val = *(uint16_t*)(memory + ofs);
	trace_load("l2s(%04x)=%08x\n", ofs, val);
	return val;
}
int8_t load1s(uint32_t ofs, uint8_t *memory) {
	int8_t val = *(int8_t*)(memory + ofs);
	trace_load("l1s(%04x)=%08x\n", ofs, val);
	return val;
}

static
void lb(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    int8_t value = load1s(addr, cpu->memory);
    trace("lb\t%s, %d(%s) \t# %02x <- @%08x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], value, addr);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

static
void lh(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    int16_t value = load2s(addr,  cpu->memory);
    trace("lh\t%s, %d(%s) \t# %04x <- @%08x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], value, addr);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

static
void lw(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint32_t value = load4(addr, cpu->memory);
    trace("lw\t%s, %d(%s) \t# %08x <- @%08x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], value, addr);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

static
void lbu(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint8_t value = load1(addr, cpu->memory);
    trace("lbu\t%s, %d(%s) \t# %02x <- @%08x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], value, addr);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

static
void lhu(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint16_t value = load2(addr, cpu->memory);
    trace("lhu\t%s, %d(%s) \t# %04x <- @%08x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], value, addr);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

static
void sb(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint8_t value = cpu->registers[rs2];
    trace("sb\t%s, %d(%s) \t# %02x -> @%08x\n", register_name[rs2], sign_expansion(imm, 12), register_name[rs1], value, addr);
    increment_pc(cpu);
    store1(addr, value, cpu->memory);
    return;
}

static
void sh(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint16_t value = cpu->registers[rs2];
    trace("sh\t%s, %d(%s) \t# %04x -> @%08x\n", register_name[rs2], sign_expansion(imm, 12), register_name[rs1], value, addr);
    increment_pc(cpu);
    store2(addr, value, cpu->memory);
    return;
}

static
void sw(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint32_t value = cpu->registers[rs2];
    trace("sw\t%s, %d(%s) \t# %08x -> @%08x\n", register_name[rs2], sign_expansion(imm, 12), register_name[rs1], value, addr);
    increment_pc(cpu);
    store4(addr, value, cpu->memory);
    return;
}

static
void addi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 + op2;
    trace("addi\t%s, %s, %d \t# %08x = %08x + %08x\n", register_name[rd], register_name[rs1], sign_expansion(imm, 12), value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void slti(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 < op2 ? 1 : 0;
    trace("slti\t%s, %s, %d\n \t# %d = %08x < %08x", register_name[rd], register_name[rs1], sign_expansion(imm, 12), value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void sltiu(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = imm;
    uint32_t value = op1 < op2 ? 1 : 0;
    trace("sltiu\t%s, %s, %d \t# %08x = %08x ^ %08x\n", register_name[rd], register_name[rs1], imm, value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void xori(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 ^ op2;
    trace("xori\t%s, %s, 0x%x \t# %08x = %08x ^ %08x\n", register_name[rd], register_name[rs1], imm, value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void ori(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 | op2;
    trace("ori\t%s, %s, 0x%x \t# %08x = %08x | %08x\n", register_name[rd], register_name[rs1], imm, value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void andi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 & op2;
    trace("andi\t%s, %s, 0x%x \t# %08x = %08x & %08x\n", register_name[rd], register_name[rs1], imm, value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void slli(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = imm & 0x1F;
    uint32_t value = op1 << op2;
    trace("slli\t%s, %s, 0x%x \t# %08x = %08x << %d\n", register_name[rd], register_name[rs1], imm, value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void srli(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = imm & 0x1F;
    uint32_t value = op1 >> op2;
    trace("srli\t%s, %s, 0x%x \t# %08x = %08x >> %d\n", register_name[rd], register_name[rs1], imm, value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void srai(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = imm & 0x1F;
    int32_t value = op1 >> op2;
    trace("srai\t%s, %s, 0x%x \t# %08x = %08x >> %d\n", register_name[rd], register_name[rs1], imm, value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void add(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    int32_t value = op1 + op2;
    trace("add\t%s, %s, %s \t# %08x = %08x + %08x\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void sub(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    int32_t value = op1 - op2;
    trace("sub\t%s, %s, %s \t# %08x = %08x - %08x\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void sll(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2] & 0x1F;
    uint32_t value = op1 << op2;
    trace("sll\t%s, %s, %s \t# %08x = %08x << %d\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void slt(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    int32_t value = op1 < op2 ? 1 : 0;
    trace("slt\t%s, %s, %s \t# %d = %d < %d\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void sltu(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 < op2 ? 1 : 0;
    trace("sltu\t%s, %s, %s \t# %d = %08x < %08x\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void xor(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 ^ op2;
    trace("xor\t%s, %s, %s \t# %08x = %08x ^ %08x\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void srl(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2] & 0x1F;
    uint32_t value = op1 >> op2;
    trace("srl\t%s, %s, %s \t# %08x = %08x >> %d\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void sra(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2] & 0x1F;
    int32_t value = op1 >> op2;
    trace("sra\t%s, %s, %s \t# %d = %d >> %d\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void or(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 | op2;
    trace("or\t%s, %s, %s \t# %08x = %08x | %08x\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void and(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 & op2;
    trace("and\t%s, %s, %s \t# %08x = %08x & %08x\n", register_name[rd], register_name[rs1], register_name[rs2], value, op1, op2);
    store_rd(cpu, rd, value);
    return;
}

static
void fence_decode(uint8_t i, char* buf) {
	if (i & (1<<3)) strcat(buf, "i");
	if (i & (1<<2)) strcat(buf, "o");
	if (i & (1<<1)) strcat(buf, "r");
	if (i & (1<<0)) strcat(buf, "w");
}

static
void fence(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    char predecessor_as_str[5] = {0};
    char successor_as_str[5] = {0};
    uint8_t predecessor = (imm >> 12) & 0xF;
    uint8_t successor = (imm >> 8) & 0xF;
    fence_decode(predecessor, predecessor_as_str);
    fence_decode(successor, successor_as_str);
    trace("fence\t%s, %s, %x \t# %s, %s\n", register_name[rd], register_name[rs1], imm, predecessor_as_str, successor_as_str);
    return;
}

__attribute__((weak))
void ecall_callback(Cpu* cpu) {
}

__attribute__((weak))
void ebreak_callback(Cpu* cpu) {
}

static
void ecall(Cpu* cpu) {
    increment_pc(cpu);
    trace("ecall \t\t\t# %s:%x\n", register_name[10], cpu->registers[10]);
    ecall_callback(cpu);
    return;
}

static
void ebreak(Cpu* cpu) {
    increment_pc(cpu);
    trace("ebreak \t#\n");
    ebreak_callback(cpu);
    return;
}

static
void csrrw(Cpu* cpu, uint32_t csr, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    trace("csrrw\t%s, %s, %x \t#\n", register_name[rd], register_name[rs1], csr);
    return;
}

__attribute__((weak))
uint64_t times() {
	return 0;
}

static
void csrrs(Cpu* cpu, uint32_t csr, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    uint64_t c = cpu->cycles;
    uint64_t t = times();
    uint32_t value ;
    switch(csr) {
	    case 0xC00:
	    case 0xC02:
		    value = c;
		    break;
	    case 0xC80:
	    case 0xC82:
		    value = c >> 32;
		    break;
	    case 0xC01:
		    value = t;
		    break;
	    case 0xC81:
		    value = t >> 32;
		    break;
	    default:
		    value = 0;
		    break;
    }
    trace("csrrs\t%s, %s, %x \t# %x\n", register_name[rd], register_name[rs1], csr, value);
    store_rd(cpu, rd, value);
    return;
}

static
void csrrc(Cpu* cpu, uint32_t csr, uint8_t rs1, uint8_t rd) {
    increment_pc(cpu);
    trace("csrrc\t%s, %s, %x \t#\n", register_name[rd], register_name[rs1], csr);
    return;
}

static
void csrrwi(Cpu* cpu, uint32_t csr, uint8_t zimm, uint8_t rd) {
    increment_pc(cpu);
    trace("csrrwi\t%s, %x, %x \t#\n", register_name[rd], zimm, csr);
    return;
}

static
void csrrsi(Cpu* cpu, uint32_t csr, uint8_t zimm, uint8_t rd) {
    increment_pc(cpu);
    trace("csrrsi\t%s, %x, %x \t#\n", register_name[rd], zimm, csr);
    return;
}

static
void csrrci(Cpu* cpu, uint32_t csr, uint8_t zimm, uint8_t rd) {
    increment_pc(cpu);
    trace("csrrci\t%s, %x, %x \t#\n", register_name[rd], zimm, csr);
    return;
}

__attribute__((weak))
__attribute__((noreturn))
void unsupported_opcode(Cpu* cpu, uint32_t instruction) {
    exit(-1);
}

void execution(Cpu* cpu, uint32_t instruction) {
    uint8_t opcode = (instruction >> 0) & 0x7F;
    uint8_t funct3 = (instruction >> 12) & 0x07;
    uint8_t funct7 = (instruction >> 25) & 0x7F;

    uint8_t rd = (instruction >> 7) & 0x1F;
    uint8_t rs1 = (instruction >> 15) & 0x1F;
    uint8_t rs2 = (instruction >> 20) & 0x1F;

    uint32_t imm;

    switch (opcode) {
        case 0b0110111: // LUI
            imm = (instruction >> 12) & 0x0FFFFF;
            lui(cpu, imm, rd);
            break;
        case 0b0010111: // AUIPC
            imm = (instruction >> 12) & 0x0FFFFF;
            auipc(cpu, imm, rd);
            break;
        case 0b1101111: // JAL
            imm = ((instruction >> 11) & 0x100000) | (instruction & 0x0FF000) |
                  ((instruction >> 9) & 0x000800) |
                  ((instruction >> 20) & 0x0007FE);
            jal(cpu, imm, rd);
            break;
        case 0b1100111: // JALR
            imm = (instruction >> 20) & 0x0FFF;
            jalr(cpu, imm, rs1, rd);
            break;
        case 0b1100011: // B*
            imm = ((instruction >> 19) & 0x1000) |
                  ((instruction << 4) & 0x800) | ((instruction >> 20) & 0x7E0) |
                  ((instruction >> 7) & 0x1E);
            switch (funct3) {
                case 0b000:
                    beq(cpu, imm, rs2, rs1);
                    break;
                case 0b001:
                    bne(cpu, imm, rs2, rs1);
                    break;
                case 0b100:
                    blt(cpu, imm, rs2, rs1);
                    break;
                case 0b101:
                    bge(cpu, imm, rs2, rs1);
                    break;
                case 0b110:
                    bltu(cpu, imm, rs2, rs1);
                    break;
                case 0b111:
                    bgeu(cpu, imm, rs2, rs1);
                    break;
                default:
                    trace("B* Unimplemented\n");
                    unsupported_opcode(cpu, instruction);
                    break;
            }
            break;
        case 0b0000011: // L*
            imm = (instruction >> 20) & 0x0FFF;
            switch (funct3) {
                case 0b000:
                    lb(cpu, imm, rs1, rd);
                    break;
                case 0b001:
                    lh(cpu, imm, rs1, rd);
                    break;
                case 0b010:
                    lw(cpu, imm, rs1, rd);
                    break;
                case 0b100:
                    lbu(cpu, imm, rs1, rd);
                    break;
                case 0b101:
                    lhu(cpu, imm, rs1, rd);
                    break;
                default:
                    trace("L* Unimplemented\n");
                    unsupported_opcode(cpu, instruction);
                    break;
            }
            break;
        case 0b0100011: // S*
            imm = ((instruction >> 20) & 0xFE0) | ((instruction >> 7) & 0x1F);
            switch (funct3) {
                case 0b000:
                    sb(cpu, imm, rs2, rs1);
                    break;
                case 0b001:
                    sh(cpu, imm, rs2, rs1);
                    break;
                case 0b010:
                    sw(cpu, imm, rs2, rs1);
                    break;
                default:
                    trace("S* Unimplemented\n");
                    unsupported_opcode(cpu, instruction);
                    break;
            }
            break;
        case 0b0010011: // Arithmetic Immediate
            imm = (instruction >> 20) & 0x0FFF;
            switch (funct3) {
                case 0b000:
                    addi(cpu, imm, rs1, rd);
                    break;
                case 0b010:
                    slti(cpu, imm, rs1, rd);
                    break;
                case 0b011:
                    sltiu(cpu, imm, rs1, rd);
                    break;
                case 0b100:
                    xori(cpu, imm, rs1, rd);
                    break;
                case 0b110:
                    ori(cpu, imm, rs1, rd);
                    break;
                case 0b111:
                    andi(cpu, imm, rs1, rd);
                    break;
                case 0b001:
                    slli(cpu, imm, rs1, rd);
                    break;
                case 0b101:
                    if (funct7 == 0) {
                        srli(cpu, imm, rs1, rd);
                    } else {
                        srai(cpu, imm, rs1, rd);
                    }
                    break;
                default:
                    trace("I* Unimplemented\n");
                    unsupported_opcode(cpu, instruction);
                    break;
            }
            break;
        case 0b0110011: // Arithmetic Registers
            switch (funct3) {
                case 0b000:
                    if (funct7 == 0) {
                        add(cpu, rs2, rs1, rd);
                    } else {
                        sub(cpu, rs2, rs1, rd);
                    }
                    break;
                case 0b001:
                    sll(cpu, rs2, rs1, rd);
                    break;
                case 0b010:
                    slt(cpu, rs2, rs1, rd);
                    break;
                case 0b011:
                    sltu(cpu, rs2, rs1, rd);
                    break;
                case 0b100:
                    xor(cpu, rs2, rs1, rd);
                    break;
                case 0b101:
                    if (funct7 == 0) {
                        srl(cpu, rs2, rs1, rd);
                    } else {
                        sra(cpu, rs2, rs1, rd);
                    }
                    break;
                case 0b110:
                    or(cpu, rs2, rs1, rd);
                    break;
                case 0b111:
                    and(cpu, rs2, rs1, rd);
                    break;
                default:
                    trace("R* Unimplemented\n");
                    unsupported_opcode(cpu, instruction);
                    break;
            }
            break;
        case 0b0001111: // FENCE
            imm = (instruction >> 12) & 0x0FFFFF;
	    fence(cpu, imm, rs1, rd);
            break;
        case 0b1110011: // ECALL/EBREAK
            imm = (instruction >> 20) & 0x0FFF;
            switch (funct3) {
                case 0b000: // ECALL/EBREAK
                    if (imm == 0) {
			ecall(cpu);
                    } else {
			ebreak(cpu);
                    }
                    break;
                case 0b001:
                    csrrw(cpu, imm, rs1, rd);
		    break;
                case 0b010:
                    csrrs(cpu, imm, rs1, rd);
		    break;
                case 0b011:
                    csrrc(cpu, imm, rs1, rd);
		    break;
                case 0b101:
                    csrrwi(cpu, imm, rs1, rd);
		    break;
                case 0b110:
                    csrrsi(cpu, imm, rs1, rd);
		    break;
                case 0b111:
                    csrrci(cpu, imm, rs1, rd);
		    break;
	    }
            break;
        default: // Unknown opco,de
            trace("** Unimplemented : opcode %x\n", opcode);
            unsupported_opcode(cpu, instruction);
            break;
    }

    // zero register is always ZERO, like /dev/zero
    _assert(! cpu->registers[0]);
    return;
}
