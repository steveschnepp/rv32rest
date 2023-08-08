PROGRAM = rv32i_emu
SRCS = main.c
OBJS = $(SRCS:.c=.o)
OPTIM = 0
DEBUG = -DDEBUG
CFLAGS = -Werror -Wall
CFLAGS += -O$(OPTIM) -g
CFLAGS += $(DEBUG)
CC = gcc

SUBDIRS := sample mmap
TOPTARGETS := all clean

all: $(PROGRAM)

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS)

.c: *.h
.c.o:
	$(CC) $(CFLAGS) -c $<

clean: sample
	rm -f $(OBJS) $(PROGRAM)

DEPS = $(SRCS:.c=.d)
include $(DEPS)

%.d : %.c
	$(CC) $(CCFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"

# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency

.PHONY: $(TOPTARGETS) $(SUBDIRS)
