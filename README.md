# RV32I Emulator

This is a RISC-V (RV32I) emulator written in C language.
Implemented all 47 instructions except CSR instructions, `ECALL` & `EBREAK`.

__It is a fork of https://github.com/zyu-c/Rv32iEmulator as I want to take it into another direction__

## Compilation


It is very easy.


``` bash
$ sudo apt -y install gcc-riscv64-unknown-elf picolibc-riscv64-unknown-elf
$ git clone https://github.com/steveschnepp/rv32rest.git
$ cd rv32rest
$ make
```

## Usage

The `sample/Makefile` builds 2 files, an `.elf` and a `.bin`.

Give a `.bin` file as an argument to `rv32i_emu`, along with the RAM, IN and OUT files.

``` bash
$ rv32i_emu sample/test1.bin mmap/ram mmap/in mmap/out
```

You can also disassemble the `.elf` file to check the generated code with the following command.

``` bash
$ riscv64-unknown-elf-objdump -D sample/empty.elf
```
