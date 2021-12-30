#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROM_SIZE 1024
#define RAM_SIZE 1024
#define MEM_SIZE ROM_SIZE + RAM_SIZE
#define ROM_SIZE_32 (ROM_SIZE / sizeof(uint32_t))
#define RAM_SIZE_32 (RAM_SIZE / sizeof(uint32_t))
#define MEM_SIZE_32 (MEM_SIZE / sizeof(uint32_t))

typedef struct {
    uint32_t pc;
    uint32_t registers[32];
    uint32_t memory[MEM_SIZE_32];
} Cpu;

void initCpu(Cpu* cpu) {
    cpu->pc = 0x0000;
    memset(cpu->registers, 0, sizeof(cpu->registers));
    memset(cpu->memory, 0, sizeof(cpu->memory));
    return;
}

uint32_t fetch(Cpu* cpu) { return cpu->memory[cpu->pc / 4]; }

void jal(Cpu* cpu, uint32_t imm, uint8_t rd) {
    if (imm >> 20) {
        imm |= 0xFFE00000;
    }
    cpu->pc += (int32_t)imm;
    cpu->registers[rd] = cpu->pc + 4;
    printf("jal\treg[%02d], %x\n", rd, cpu->pc);
    return;
}

void addi(Cpu* cpu, uint32_t imm, uint8_t rs1, uint8_t rd) {
    cpu->pc += 4;
    cpu->registers[rd] = cpu->registers[rs1] + imm;
    printf("addi\treg[%02d], reg[%02d], %d\n", rd, rs1, imm);
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

int main(int argc, char* argv[]) {
    Cpu cpu;
    initCpu(&cpu);

    //バイナリファイルが指定されていない場合
    if (argc != 2) {
        printf("usage: rv32i_emu <binary file>\n");
        return 1;
    }

    //バイナリファイルの読み込み
    FILE* binary;

    binary = fopen(argv[1], "rb");
    if (binary == NULL) {
        printf("Can't open binary file.\n");
        return 1;
    }

    fread(cpu.memory, sizeof(uint32_t), RAM_SIZE_32, binary);
    fclose(binary);

    //メモリダンプ
    // for (int i = 0; i < RAM_SIZE_32; i++) {
    //     printf("0x%04x : 0x%08x\n", i * 4, cpu.memory[i]);
    // }

    uint32_t instruction;
    while (1) {
        instruction = fetch(&cpu);
        execution(&cpu, instruction);

        if (cpu.pc == 0x0000) {
            break;
        }
    }

    //レジスタダンプ
    for (int i = 0; i < 32; i++) {
        printf("reg[%02d]:0x%08x\n", i, cpu.registers[i]);
    }

    printf("PC     :0x%08x\n", cpu.pc);

    return 0;
}
