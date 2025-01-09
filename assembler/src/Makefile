# Makefile per l'assembler modulare
CC = gcc
CFLAGS = -Wall -I../include

# Trova tutti i file .c presenti in questa cartella
SRCS = $(wildcard *.c)
# Produce gli .o corrispondenti
OBJS = $(SRCS:.c=.o)

TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
