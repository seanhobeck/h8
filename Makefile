CC=gcc 
TARGET=h8
SOURCES=src/main.c src/lex/lex.c src/emu/emu.c
CFLAGS=-g -Wall
LFLAGS=-std=c11

OBJS=$(SOURCES:.c=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(TARGET)

purge: clean
	rm -f $(TARGET)

clean:
	rm -f src/*.o src/lex/*.o