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
    uint32_t registers[32];
    uint32_t memory[MEM_SIZE_32];
} Cpu;

void initCpu(Cpu* cpu) {
    memset(cpu->registers, 0, sizeof(cpu->registers));
    memset(cpu->memory, 0, sizeof(cpu->memory));
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
    for (int i = 0; i < RAM_SIZE_32; i++) {
        printf("0x%04x : 0x%08x\n", i * 4, cpu.memory[i]);
    }

    return 0;
}
