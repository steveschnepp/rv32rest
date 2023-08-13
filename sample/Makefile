SRCS=$(wildcard *.c)
ELFS=$(SRCS:.c=.elf)
BINS=$(SRCS:.c=.bin)

CC=riscv64-unknown-elf-gcc
OBJCOPY=riscv64-unknown-elf-objcopy

ARCH=rv32e
ABI=ilp32e
OPTIM=0
CFLAGS=-march=$(ARCH) -mabi=$(ABI)
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

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -O srec $< $@

clean:
	rm -f $(BINS) $(ELFS)
