# RV32I Emulator
これはC言語で記述したRISC-V(RV32I)のエミュレータです．
CSR命令, `ECALL`，`EBREAK`を除く47個の命令を実装しました．

## 使い方
以下のコマンドで`rv32i_emu`という実行ファイルが生成されます．
``` bash
$ git clone https://github.com/zyu-c/Rv32iEmulator.git
$ cd Rv32iEmulator
$ make
```

実行時は引数にRV32I用にコンパイルされたバイナリファイルを与えてください．
``` bash
$ rv32i_emu sample/test1.bin
```

## サンプルファイルのコンパイル
予めRISC-Vのコンパイル環境を整えておいてください．
詳しくは→[riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain)

サンプルプログラムのコンパイルを行い，elfファイルを生成します．
``` bash
$ cd sample
$ riscv32-unknown-linux-gnu-gcc -o test1.elf test1.c start.s -march=rv32i -static -nostdlib -nostartfiles -T link.ld
```

elfファイルをバイナリファイルに変換します．
``` bash
$ riscv32-unknown-linux-gnu-objcopy -O binary test1.elf test1.bin
```

以下のコマンドでelfファイルを逆アセンブルすることができます．
``` bash
$ riscv32-unknown-linux-gnu-objdump -d test1.elf
```
