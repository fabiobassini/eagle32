# ------------------------------------------------------------------------------
# Makefile per emulatoreCPU
# ------------------------------------------------------------------------------

# Compilatore e flag
CC       = gcc
CFLAGS   = -Wall -Wextra -I./include -I./include/cpu -I./include/bus -I./include/graphics -I./include/io -I./include/memory
# Se vuoi il debug, aggiungi -g:
# CFLAGS  += -g

# Flags di librerie SDL e OpenGL
# Puoi usare sdl2-config per ottenere i parametri:
SDL_FLAGS = $(shell sdl2-config --cflags --libs) -framework OpenGL

# Nome del target
TARGET = emulator

# ------------------------------------------------------------------------------
# Individuazione automatica dei file .c nella cartella src
# ------------------------------------------------------------------------------
SRCS = $(shell find src -name "*.c")

# Conversione .c in .o
OBJS = $(SRCS:.c=.o)

# ------------------------------------------------------------------------------
# Regola di compilazione principale
# ------------------------------------------------------------------------------
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_FLAGS)

# ------------------------------------------------------------------------------
# Regola di compilazione di un .c in .o
# Usa i flag e le include definite sopra
# ------------------------------------------------------------------------------
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------------------------------
# Pulizia
# ------------------------------------------------------------------------------
clean:
	rm -f $(OBJS) $(TARGET)

# ------------------------------------------------------------------------------
# Install / Dist / ecc. (se servono)
# Se non usi, puoi rimuovere
# ------------------------------------------------------------------------------
install:
	@echo "Nessuna regola di install definita."

dist:
	@echo "Nessuna regola di dist definita."

# ------------------------------------------------------------------------------
# Fine Makefile
# ------------------------------------------------------------------------------
