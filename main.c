#include <stdint.h>
#include <stdio.h>

#include "cpu.h"
#include "instruction.h"
#include "log.h"

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

    uint32_t instruction;
    while (1) {
        instruction = fetch(&cpu);
        execution(&cpu, instruction);

        if (cpu.pc == 0x0000) {
            break;
        }
    }

    dumpRegister(&cpu);

    return 0;
}
