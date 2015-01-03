CC = gcc

CFLAGS = -Wall -Wextra -larchive -lncurses

SRC = $(wildcard *.c)
BIN = $(patsubst %.c,%,$(SRC))

all: $(BIN)

clean:
	$(RM) $(BIN)
	$(RM) a.out



