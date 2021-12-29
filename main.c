#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    //メモリ 1kB
    uint32_t memory[256];

    //バイナリファイルが指定されていない場合
    if (argc != 2) {
        printf("usage: rv32i_emu <binary file>\n");
        return 1;
    }

    //バイナリファイルの読み込み
    FILE *binary;

    binary = fopen(argv[1], "rb");
    if (binary == NULL) {
        printf("Can't open binary file.\n");
        return 1;
    }

    fread(memory, sizeof(uint32_t), 128, binary);
    fclose(binary);

    //メモリダンプ
    for (int i = 0; i < 128; i++) {
        printf("0x%04x : 0x%08x\n", i * 4, memory[i]);
    }

    return 0;
}
