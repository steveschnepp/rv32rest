#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>
#include <stdio.h>

#include "cpu.h"

#ifdef DEBUG
#define trace printf
#else
void trace(char *c, ...) { }
#endif

#ifdef DEBUG_LOAD
#define trace_load printf
#else
void trace_load(char *c, ...) { }
#endif

#ifdef DEBUG_STORE
#define trace_store printf
#else
void trace_store(char *c, ...) { }
#endif

#ifndef sign_expansion
#define sign_expansion sign_expansion_for
#endif

// Code expansion
int32_t sign_expansion_sw(uint32_t imm, uint8_t width) {
    int32_t s;
	switch(width) {
	    default:
		    assert(0);
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

void set_pc(Cpu* cpu, uint32_t pc) {
    cpu->pc = pc;
}

void increment_pc(Cpu* cpu) {
    set_pc(cpu, cpu->pc + 4);
}

void store_rd(Cpu* cpu, uint8_t rd, uint32_t value) {
    if (rd) {
        cpu->registers[rd] = value;
    } else trace("\t write to zero ignored\n");
}

void lui(Cpu* cpu, uint32_t imm, uint8_t rd) {
    trace("lui\t%s, 0x%x\n", register_name[rd], imm);
    store_rd(cpu, rd, imm << 12);
    increment_pc(cpu);
    return;
}

void auipc(Cpu* cpu, uint32_t imm, uint8_t rd) {
    trace("auipc\t%s, 0x%x\n", register_name[rd], imm);
    store_rd(cpu, rd, cpu->pc + (imm << 12));
    increment_pc(cpu);
    return;
}

void jal(Cpu* cpu, uint32_t imm, uint8_t rd) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 20);
    trace("jal\t%s, %x\n", register_name[rd], addr);
    store_rd(cpu, rd, cpu->pc + 4);
    set_pc(cpu, addr);
    return;
}

void jalr(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    trace("jalr\t%s, %s, %x\n", register_name[rd], register_name[rs1], addr);
    store_rd(cpu, rd, cpu->pc + 4);
    set_pc(cpu, addr);
    return;
}

void beq(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    trace("beq\t%s, %s, %x\n", register_name[rs1], register_name[rs2], addr);
    if (op1 == op2) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    return;
}

void bne(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    trace("bne\t%s, %s, %x\n", register_name[rs1], register_name[rs2], addr);
    if (op1 != op2) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    return;
}

void blt(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    trace("blt\t%s, %s, %x\n", register_name[rs1], register_name[rs2], addr);
    if (op1 < op2) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    return;
}

void bge(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    trace("bge\t%s, %s, %x\n", register_name[rs1], register_name[rs2], addr);
    if (op1 >= op2) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    return;
}

void bltu(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    trace("bltu\t%s, %s, %x\n", register_name[rs1], register_name[rs2], addr);
    if (op1 < op2) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    return;
}

void bgeu(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->pc + sign_expansion(imm, 13);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    trace("bgeu\t%s, %s, %x\n", register_name[rs1], register_name[rs2], addr);
    if (op1 >= op2) {
        set_pc(cpu, addr);
    } else {
        increment_pc(cpu);
    }
    return;
}

void store4(uint32_t ofs, uint32_t val, uint8_t *memory) {
	trace_store("s4u(%04x)=%08x\n", ofs, val);
	*(uint32_t*)(memory + ofs) = val;
}
void store2(uint32_t ofs, uint16_t val, uint8_t *memory) {
	trace_store("s2u(%04x)=%08x\n", ofs, val);
	*(uint16_t*)(memory + ofs) = val;
}
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

void lb(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    int8_t value = load1s(addr, cpu->memory);
    trace("lb\t%s, %d(%s) \t# @%08x -> %02x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], addr, value);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

void lh(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    int16_t value = load2s(addr,  cpu->memory);
    trace("lh\t%s, %d(%s) \t# @%08x -> %04x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], addr, value);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

void lw(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint32_t value = load4(addr, cpu->memory);
    trace("lw\t%s, %d(%s) \t# @%08x -> %08x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], addr, value);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

void lbu(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint8_t value = load1(addr, cpu->memory);
    trace("lbu\t%s, %d(%s) \t# @%08x -> %02x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], addr, value);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

void lhu(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint16_t value = load2(addr, cpu->memory);
    trace("lhu\t%s, %d(%s) \t# @%08x -> %04x\n", register_name[rd], sign_expansion(imm, 12), register_name[rs1], addr, value);
    increment_pc(cpu);
    store_rd(cpu, rd, value);
    return;
}

void sb(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint8_t value = cpu->registers[rs2];
    trace("sb\t%s, %d(%s) \t# @%08x <- %02x\n", register_name[rs2], sign_expansion(imm, 12), register_name[rs1], addr, value);
    increment_pc(cpu);
    store1(addr, value, cpu->memory);
    return;
}

void sh(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint16_t value = cpu->registers[rs2];
    trace("sh\t%s, %d(%s) \t# @%08x <- %04x\n", register_name[rs2], sign_expansion(imm, 12), register_name[rs1], addr, value);
    increment_pc(cpu);
    store2(addr, value, cpu->memory);
    return;
}

void sw(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    uint32_t addr = cpu->registers[rs1] + sign_expansion(imm, 12);
    uint32_t value = cpu->registers[rs2];
    trace("sw\t%s, %d(%s) \t# @%08x <- %08x\n", register_name[rs2], sign_expansion(imm, 12), register_name[rs1], addr, value);
    increment_pc(cpu);
    store4(addr, value, cpu->memory);
    return;
}

void addi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    trace("addi\t%s, %s, %d\n", register_name[rd], register_name[rs1], sign_expansion(imm, 12));
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 + op2;
    store_rd(cpu, rd, value);
    return;
}

void slti(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    trace("slti\t%s, %s, %d\n", register_name[rd], register_name[rs1], sign_expansion(imm, 12));
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 < op2 ? 1 : 0;
    store_rd(cpu, rd, value);
    return;
}

void sltiu(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    trace("sltiu\t%s, %s, %d\n", register_name[rd], register_name[rs1], imm);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = imm;
    uint32_t value = op1 < op2 ? 1 : 0;
    store_rd(cpu, rd, value);
    return;
}

void xori(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    trace("xori\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 ^ op2;
    store_rd(cpu, rd, value);
    return;
}

void ori(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    trace("ori\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 | op2;
    store_rd(cpu, rd, value);
    return;
}

void andi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    trace("andi\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = sign_expansion(imm, 12);
    uint32_t value = op1 & op2;
    store_rd(cpu, rd, value);
    return;
}

void slli(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    trace("slli\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = imm & 0x1F;
    uint32_t value = op1 << op2;
    store_rd(cpu, rd, value);
    return;
}

void srli(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    trace("srli\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = imm & 0x1F;
    uint32_t value = op1 >> op2;
    store_rd(cpu, rd, value);
    return;
}

void srai(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    trace("srai\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = imm & 0x1F;
    int32_t value = op1 >> op2;
    store_rd(cpu, rd, value);
    return;
}

void add(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("add\t%s, %s, %s\n", register_name[rd], register_name[rs1], register_name[rs2]);
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    int32_t value = op1 + op2;
    store_rd(cpu, rd, value);
    return;
}

void sub(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("sub\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    int32_t value = op1 - op2;
    store_rd(cpu, rd, value);
    return;
}

void sll(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("sll\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2] & 0x1F;
    uint32_t value = op1 << op2;
    store_rd(cpu, rd, value);
    return;
}

void slt(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("slt\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    int32_t value = op1 < op2 ? 1 : 0;
    store_rd(cpu, rd, value);
    return;
}

void sltu(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("sltu\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 < op2 ? 1 : 0;
    store_rd(cpu, rd, value);
    return;
}

void xor(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("xor\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 ^ op2;
    store_rd(cpu, rd, value);
    return;
}

void srl(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("srl\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 >> op2;
    store_rd(cpu, rd, value);
    return;
}

void sra(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("sra\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    int32_t op1 = cpu->registers[rs1];
    int32_t op2 = cpu->registers[rs2];
    int32_t value = op1 >> op2;
    store_rd(cpu, rd, value);
    return;
}

void or(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("or\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 | op2;
    store_rd(cpu, rd, value);
    return;
}

void and(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    trace("and\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    increment_pc(cpu);
    uint32_t op1 = cpu->registers[rs1];
    uint32_t op2 = cpu->registers[rs2];
    uint32_t value = op1 & op2;
    store_rd(cpu, rd, value);
    return;
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
        case 0b0110111:
            imm = (instruction >> 12) & 0x0FFFFF;
            lui(cpu, imm, rd);
            break;
        case 0b0010111:
            imm = (instruction >> 12) & 0x0FFFFF;
            auipc(cpu, imm, rd);
            break;
        case 0b1101111:
            imm = ((instruction >> 11) & 0x100000) | (instruction & 0x0FF000) |
                  ((instruction >> 9) & 0x000800) |
                  ((instruction >> 20) & 0x0007FE);
            jal(cpu, imm, rd);
            break;
        case 0b1100111:
            imm = (instruction >> 20) & 0x0FFF;
            jalr(cpu, imm, rs1, rd);
            break;
        case 0b1100011:
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
                    trace("Unimplemented\n");
                    break;
            }
            break;
        case 0b0000011:
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
                    trace("Unimplemented\n");
                    break;
            }
            break;
        case 0b0100011:
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
                    trace("Unimplemented\n");
                    break;
            }
            break;
        case 0b0010011:
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
                    trace("Unimplemented\n");
                    break;
            }
            break;
        case 0b0110011:
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
                    trace("Unimplemented\n");
                    break;
            }
            break;
        default:
            trace("Unimplemented\n");
            break;
    }

    // zero register is always ZERO, like /dev/zero
    assert(! cpu->registers[0]);
    return;
}

#endif
