CC = gcc
CFLAGS = -Wall -Wextra -std=c11
EXE = a5
SRCS = a5.c

all: $(EXE)

$(EXE): $(SRCS)
	$(CC) $(CFLAGS) -o $(EXE) $(SRCS)

clean:
	rm -f $(EXE)

.PHONY: all clean
