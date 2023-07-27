# RV32I Emulator

This is a RISC-V (RV32I) emulator written in C language.
Implemented 37 instructions except `FENCE`, `ECALL` & `EBREAK`.

## Usage

The following command `rv32i_emu` will generate an executable file.

``` bash
$ git clone https://github.com/zyu-c/Rv32iEmulator.git
$ cd Rv32iEmulator
$ make
```

Give a binary file compiled for RV32I as an argument when executing.

``` bash
$ rv32i_emu sample/test1.bin
```

## Compiling the sample files

Prepare the RISC-V compilation environment in advance.
For details → [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain).

Compile the sample program and generate an ELF file.

``` bash
$ cd sample
$ riscv32-unknown-linux-gnu-gcc -o test1.elf test1.c start.s -march=rv32i -static -nostdlib -nostartfiles -T link.ld
```

Convert an elf file to a binary file.

``` bash
$ riscv32-unknown-linux-gnu-objcopy -O binary test1.elf test1.bin
```

You can disassemble the elf file with the following command.

``` bash
$ riscv32-unknown-linux-gnu-objdump -d test1.elf
```
