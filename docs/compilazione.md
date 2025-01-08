# Guida alla Compilazione del Progetto

Questa guida spiega come compilare l'emulatore e l'assembler usando Make, includendo i prerequisiti e i comandi da eseguire.

## Prerequisiti

- **GCC**: Il compilatore C.
- **SDL2**: La libreria SDL2 deve essere installata (sia i runtime che i pacchetti di sviluppo).
- **Make**: Per eseguire i comandi di compilazione automatici.

Assicurarsi che i comandi `gcc`, `make` e `sdl2-config` siano presenti nel PATH.

## Compilazione dell'Emulatore

1. Aprire un terminale nella directory radice del progetto (`emulatore/`).
2. Eseguire il comando:
   ```bash
   make
