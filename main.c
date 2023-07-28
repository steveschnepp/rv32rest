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
    int loop_cnt = 0;

    while (1) {
        instruction = fetch(&cpu);
        printf("%4x: %08x\t\t", cpu.pc, instruction);

        execution(&cpu, instruction);

        loop_cnt++;
        if (loop_cnt > 100000) {
            //ループ回数が多すぎる場合は停止する
            printf("Error.\n");
            break;
        }

        if (cpu.pc == 0x0000) {
            //プログラムカウンタが0に戻ったら終了
            printf("Finish.\n");
            break;
        }
    }

    dumpRegister(&cpu);
    dumpMemory(&cpu);

    return 0;
}
