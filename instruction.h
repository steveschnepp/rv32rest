#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <stdint.h>
#include <stdio.h>

#include "cpu.h"

int32_t signExtension(uint32_t imm_20) {
    int32_t imm;
    if (imm_20 >> 20) {
        imm = imm_20 | 0xFFE00000;
    } else {
        imm = imm_20;
    }
    return imm;
}

void jal(Cpu* cpu, uint32_t imm, uint8_t rd) {
    cpu->pc += signExtension(imm);
    cpu->registers[rd] = cpu->pc + 4;
    printf("jal\t%s, %x\n", register_name[rd], cpu->pc);
    return;
}

void addi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    cpu->pc += 4;
    cpu->registers[rd] = cpu->registers[rs1] + imm;
    printf("addi\t%s, %s, %d\n", register_name[rd], register_name[rs1], imm);
    return;
}

void xori(Cpu* cpu, uint16_t imm, uint8_t rs1, uint8_t rd) {
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1] ^ imm) & 0x0FFF;
    return;
}

void ori(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1] | imm) & 0x0FFF;
    return;
}

void andi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    cpu->pc += 4;
    cpu->registers[rd] = (cpu->registers[rs1] & imm) & 0x0FFF;
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
        case 0b1101111:
            imm = ((instruction >> 11) & 0x100000) | (instruction & 0x0FF000) |
                  ((instruction >> 9) & 0x000800) |
                  ((instruction >> 20) & 0x0007FE);
            jal(cpu, imm, rd);
            break;
        case 0b0010011:
            imm = (instruction >> 20) & 0x0FFF;
            switch (funct3) {
                case 0b000:
                    addi(cpu, imm, rs1, rd);
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
                default:
                    printf("未実装\n");
                    return;
                    break;
            }
            break;
        default:
            printf("未実装\n");
            return;
            break;
    }
    return;
}

#endif
