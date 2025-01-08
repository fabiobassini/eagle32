CC = gcc
CFLAGS = -Wall -I./src
SDL_FLAGS = $(shell sdl2-config --cflags --libs) -framework OpenGL
SRCS = $(shell find src -name "*.c")
OBJS = $(SRCS:.c=.o)
TARGET = emulator

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_FLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
