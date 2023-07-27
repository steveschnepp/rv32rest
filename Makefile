PROGRAM = rv32i_emu
SRCS = main.c
OBJS = $(SRCS:.c=.o)
CFLAG = -Wall -O0 -g
CC = gcc

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAG) -o $(PROGRAM) $(OBJS)

.c: *.h
.c.o:
	$(CC) $(CFLAG) -c $<

clean:
	rm -f $(OBJS) $(PROGRAM)
