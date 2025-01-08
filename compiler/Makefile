CC = gcc
CFLAGS = -Wall -I./src
SRCS = $(shell find src -name "*.c")
OBJS = $(SRCS:.c=.o)
TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
