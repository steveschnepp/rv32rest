SRCS=$(wildcard *.c)
ELFS=$(SRCS:.c=.elf)
BINS=$(SRCS:.c=.bin)

CC=riscv64-unknown-elf-gcc
OBJCOPY=riscv64-unknown-elf-objcopy

ARCH=rv32e
ABI=ilp32e
OPTIM=g
CFLAGS=-march=$(ARCH) -mabi=$(ABI)
CFLAGS+=-D__RV32REST__
CFLAGS+=-g
CFLAGS+=-O$(OPTIM)
CFLAGS+=--specs=picolibc.specs
CFLAGS+=$(EXTRA_CFLAGS)

LDFLAGS+=-static
LDFLAGS+=-march=$(ARCH) -mabi=$(ABI)
LDFLAGS+=--specs=picolibc.specs
LDFLAGS+=-T sample.ld
LDFLAGS+=-Wl,--print-memory-usage

all: $(BINS)

elf: $(ELFS)

%.elf: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

test1.elf: LDFLAGS=-static -nostdlib -nostartfiles -T link.ld -march=$(ARCH) -mabi=$(ABI) start.s

sieve.elf: bitset.o
bitset.elf  ecall.elf  empty.elf  fence.elf  fibo.elf  forth8.elf csr.elf  pi.elf printf.elf  sieve.elf  test1.elf  test1bis.elf  test2.elf  test3.elf  testloop.elf: ecall.o

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -O srec $< $@

clean:
	rm -f $(BINS) $(ELFS)
