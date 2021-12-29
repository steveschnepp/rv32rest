PROGRAM = rv32i_emu
SRCS = main.c
OBJS = $(SRCS:.c=.o)
CFLAG = -Wall -O2
CC = gcc

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAG) -o $(PROGRAM) $(OBJS)

.c.o:
	$(CC) $(CFLAG) -c $<

clean:
	rm -f $(OBJS) $(PROGRAM)
