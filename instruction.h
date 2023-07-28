#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>
#include <stdio.h>

#include "cpu.h"

//符号拡張
int32_t signExtension(uint32_t imm, uint8_t width) {
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

void lui(Cpu* cpu, uint32_t imm, uint8_t rd) {
    printf("lui\t%s, 0x%x\n", register_name[rd], imm);
    cpu->registers[rd] = imm << 12;
    cpu->pc += 4;
    return;
}

void auipc(Cpu* cpu, uint32_t imm, uint8_t rd) {
    printf("auipc\t%s, 0x%x\n", register_name[rd], imm);
    cpu->registers[rd] = cpu->pc + (imm << 12);
    cpu->pc += 4;
    return;
}

void jal(Cpu* cpu, uint32_t imm, uint8_t rd) {
    printf("jal\t%s, %x\n", register_name[rd],
           cpu->pc + signExtension(imm, 20));
    cpu->registers[rd] = cpu->pc + 4;
    cpu->pc += signExtension(imm, 20);
    return;
}

void jalr(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("jalr\t%s, %s, %x\n", register_name[rd], register_name[rs1],
           cpu->registers[rs1] + signExtension(imm, 12));
    cpu->registers[rd] = cpu->pc + 4;
    cpu->pc = cpu->registers[rs1] + signExtension(imm, 12);
    return;
}

void beq(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("beq\t%s, %s, %x\n", register_name[rs1], register_name[rs2],
           cpu->pc + signExtension(imm, 13));
    if (cpu->registers[rs1] == cpu->registers[rs2]) {
        cpu->pc += signExtension(imm, 13);
    } else {
        cpu->pc += 4;
    }
    return;
}

void bne(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("bne\t%s, %s, %x\n", register_name[rs1], register_name[rs2],
           cpu->pc + signExtension(imm, 13));
    if (cpu->registers[rs1] != cpu->registers[rs2]) {
        cpu->pc += signExtension(imm, 13);
    } else {
        cpu->pc += 4;
    }
    return;
}

void blt(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("blt\t%s, %s, %x\n", register_name[rs1], register_name[rs2],
           cpu->pc + signExtension(imm, 13));
    if ((int32_t)(cpu->registers[rs1]) < (int32_t)(cpu->registers[rs2])) {
        cpu->pc += signExtension(imm, 13);
    } else {
        cpu->pc += 4;
    }
    return;
}

void bge(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("bge\t%s, %s, %x\n", register_name[rs1], register_name[rs2],
           cpu->pc + signExtension(imm, 13));
    if ((int32_t)(cpu->registers[rs1]) >= (int32_t)(cpu->registers[rs2])) {
        cpu->pc += signExtension(imm, 13);
    } else {
        cpu->pc += 4;
    }
    return;
}

void bltu(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("bltu\t%s, %s, %x\n", register_name[rs1], register_name[rs2],
           cpu->pc + signExtension(imm, 13));
    if (cpu->registers[rs1] < cpu->registers[rs2]) {
        cpu->pc += signExtension(imm, 13);
    } else {
        cpu->pc += 4;
    }
    return;
}

void bgeu(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("bgeu\t%s, %s, %x\n", register_name[rs1], register_name[rs2],
           cpu->pc + signExtension(imm, 13));
    if (cpu->registers[rs1] >= cpu->registers[rs2]) {
        cpu->pc += signExtension(imm, 13);
    } else {
        cpu->pc += 4;
    }
    return;
}

void store4(uint32_t ofs, uint32_t val, uint32_t *memory) {
	uint8_t* image = (uint8_t*) memory;
	printf("s4u(%04x)=%08x\n", ofs, val);
	*(uint32_t*)(image + ofs) = val;
}
void store2(uint32_t ofs, uint32_t val, uint32_t *memory) {
	uint8_t* image = (uint8_t*) memory;
	printf("s2u(%04x)=%08x\n", ofs, val);
	*(uint16_t*)(image + ofs) = val;
}
void store1(uint32_t ofs, uint32_t val, uint32_t *memory) {
	uint8_t* image = (uint8_t*) memory;
	printf("s1u(%04x)=%08x\n", ofs, val);
	*(uint8_t*)(image + ofs) = val;
}

uint32_t load4(uint32_t ofs, uint32_t *memory) {
	uint8_t* image = (uint8_t*) memory;
	uint32_t val = *(uint32_t*)(image + ofs);
	printf("l4u(%04x)=%08x\n", ofs, val);
	return val;
}
uint16_t load2(uint32_t ofs, uint32_t *memory) {
	uint8_t* image = (uint8_t*) memory;
	uint16_t val = *(uint16_t*)(image + ofs);
	printf("l2u(%04x)=%08x\n", ofs, val);
	return val;
}
uint8_t load1(uint32_t ofs, uint32_t *memory) {
	uint8_t* image = (uint8_t*) memory;
	uint8_t val = *(uint8_t*)(image + ofs);
	printf("l1u(%04x)=%08x\n", ofs, val);
	return val;
}

int16_t load2s(uint32_t ofs, uint32_t *memory) {
	uint8_t* image = (uint8_t*) memory;
	uint16_t val = *(uint16_t*)(image + ofs);
	printf("l2s(%04x)=%08x\n", ofs, val);
	return val;
}
int8_t load1s(uint32_t ofs, uint32_t *memory) {
	uint8_t* image = (uint8_t*) memory;
	int8_t val = *(int8_t*)(image + ofs);
	printf("l1s(%04x)=%08x\n", ofs, val);
	return val;
}

void lb(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("lb\t%s, %d(%s)\n", register_name[rd], signExtension(imm, 12),
           register_name[rs1]);
    cpu->pc += 4;
    uint32_t addr = cpu->registers[rs1] + signExtension(imm, 12);
    int8_t value = load1s(addr, cpu->memory);
    cpu->registers[rd] = value;
    return;
}

void lh(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("lh\t%s, %d(%s)\n", register_name[rd], signExtension(imm, 12),
           register_name[rs1]);
    cpu->pc += 4;
    uint32_t addr = cpu->registers[rs1] + signExtension(imm, 12);
    int16_t value = load2s(addr,  cpu->memory);
    cpu->registers[rd] = value;
    return;
}

void lw(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("lw\t%s, %d(%s)\n", register_name[rd], signExtension(imm, 12),
           register_name[rs1]);
    cpu->pc += 4;
    uint32_t addr = cpu->registers[rs1] + signExtension(imm, 12);
    uint32_t value = load4(addr, cpu->memory);
    cpu->registers[rd] = value;
    return;
}

void lbu(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("lbu\t%s, %d(%s)\n", register_name[rd], signExtension(imm, 12),
           register_name[rs1]);
    cpu->pc += 4;
    uint32_t addr = cpu->registers[rs1] + signExtension(imm, 12);
    uint8_t value = load1(addr, cpu->memory);
    cpu->registers[rd] = value;
    return;
}

void lhu(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("lhu\t%s, %d(%s)\n", register_name[rd], signExtension(imm, 12),
           register_name[rs1]);
    cpu->pc += 4;
    uint32_t addr = cpu->registers[rs1] + signExtension(imm, 12);
    uint16_t value = load2s(addr, cpu->memory);
    cpu->registers[rd] = value;
    return;
}

void sb(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("sb\t%s, %d(%s)\n", register_name[rs2], signExtension(imm, 12),
           register_name[rs1]);
    cpu->pc += 4;
    uint32_t addr = cpu->registers[rs1] + signExtension(imm, 12);
    uint8_t value = cpu->registers[rs2];
    store1(addr, value, cpu->memory);
    return;
}

void sh(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("sh\t%s, %d(%s)\n", register_name[rs2], signExtension(imm, 12),
           register_name[rs1]);
    cpu->pc += 4;
    uint32_t addr = cpu->registers[rs1] + signExtension(imm, 12);
    uint16_t value = cpu->registers[rs2];
    store2(addr, value, cpu->memory);
    return;
}

void sw(Cpu* cpu, uint32_t imm, uint8_t rs2, uint8_t rs1) {
    printf("sw\t%s, %d(%s)\n", register_name[rs2], signExtension(imm, 12),
           register_name[rs1]);
    cpu->pc += 4;
    uint32_t addr = cpu->registers[rs1] + signExtension(imm, 12);
    uint32_t value = cpu->registers[rs2];
    store4(addr, value, cpu->memory);
    return;
}

void addi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("addi\t%s, %s, %d\n", register_name[rd], register_name[rs1],
           signExtension(imm, 12));
    cpu->pc += 4;
    cpu->registers[rd] = cpu->registers[rs1] + signExtension(imm, 12);
    return;
}

void slti(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    printf("slti\t%s, %s, %d\n", register_name[rd], register_name[rs1],
           signExtension(imm, 12));
    cpu->pc += 4;
    cpu->registers[rd] =
        ((int32_t)(cpu->registers[rs1]) < signExtension(imm, 12)) ? 1 : 0;
    return;
}

void sltiu(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    printf("sltiu\t%s, %s, %d\n", register_name[rd], register_name[rs1], imm);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1] < imm) ? 1 : 0;
    return;
}

void xori(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    printf("xori\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1] ^ imm) & 0x0FFF;
    return;
}

void ori(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("ori\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1] | imm) & 0x0FFF;
    return;
}

void andi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("andi\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1] & imm) & 0x0FFF;
    return;
}

void slli(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("slli\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    cpu->pc += 4;
    cpu->registers[rd] = cpu->registers[rs1] << (0x1F & imm);
    return;
}

void srli(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("srli\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    cpu->pc += 4;
    cpu->registers[rd] = cpu->registers[rs1] >> (0x1F & imm);
    return;
}

void srai(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    printf("srai\t%s, %s, 0x%x\n", register_name[rd], register_name[rs1], imm);
    cpu->pc += 4;
    cpu->registers[rd] = (int32_t)(cpu->registers[rs1]) >> (0x1F & imm);
    return;
}

void add(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("add\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] =
        (int32_t)(cpu->registers[rs1]) + (int32_t)(cpu->registers[rs2]);
    return;
}

void sub(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("sub\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] =
        (int32_t)(cpu->registers[rs1]) - (int32_t)(cpu->registers[rs2]);
    return;
}

void sll(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("sll\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1]) << (cpu->registers[rs2]);
    return;
}

void slt(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("slt\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] =
        ((int32_t)(cpu->registers[rs1]) < (int32_t)(cpu->registers[rs2])) ? 1
                                                                          : 0;
    return;
}

void sltu(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("sltu\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] =
        ((cpu->registers[rs1]) < (cpu->registers[rs2])) ? 1 : 0;
    return;
}

void xoro(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("or\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1]) ^ (cpu->registers[rs2]);
    return;
}

void srl(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("srl\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1]) << (cpu->registers[rs2]);
    return;
}

void sra(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("sra\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] = (int32_t)(cpu->registers[rs1])
                         << (cpu->registers[rs2]);
    return;
}

void oro(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("or\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1]) | (cpu->registers[rs2]);
    return;
}

void ando(Cpu* cpu, uint8_t rs2, uint8_t rs1, uint8_t rd) {
    printf("and\t%s, %s, %s\n", register_name[rd], register_name[rs1],
           register_name[rs2]);
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1]) & (cpu->registers[rs2]);
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
                    printf("未実装\n");
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
                    printf("未実装\n");
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
                    printf("未実装\n");
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
                    printf("未実装\n");
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
                    xoro(cpu, rs2, rs1, rd);
                    break;
                case 0b101:
                    if (funct7 == 0) {
                        srl(cpu, rs2, rs1, rd);
                    } else {
                        sra(cpu, rs2, rs1, rd);
                    }
                    break;
                case 0b110:
                    oro(cpu, rs2, rs1, rd);
                    break;
                case 0b111:
                    ando(cpu, rs2, rs1, rd);
                    break;
                default:
                    printf("未実装\n");
                    break;
            }
            break;
        default:
            printf("未実装\n");
            break;
    }
    return;
}

#endif
