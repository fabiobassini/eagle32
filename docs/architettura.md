# Architettura dell'Emulatore

Questo emulatore modella un semplice processore a 32 bit con:

- **Registri Generali**  
  16 registri a 32 bit (`R0..R15`), memorizzati in una struttura `Registers`.
  
- **Registri Speciali**  
  - `PC` (Program Counter), 32 bit, indica l’indirizzo della prossima istruzione.  
  - `SP` (Stack Pointer), 32 bit, puntatore allo stack in memoria.  
  - `flags` (32 bit), dove i bit significativi sono:
    - `FLAG_CARRY` (1 << 0)
    - `FLAG_ZERO`  (1 << 1)
    - `FLAG_SIGN`  (1 << 2)
    - `FLAG_OVERFLOW` (1 << 3)

- **Bus di Sistema**  
  - Un **bus unificato** per leggere/scrivere word (32 bit) dalla memoria e da dispositivi di I/O.  
  - Indirizzi `< MEMORY_SIZE` (1 MB di default) accedono alla **RAM**.  
  - Indirizzi `>= MEMORY_SIZE` accedono a porte **I/O** (simulate).

- **Memoria**  
  - 1 MB di RAM (indirizzabile da 0 a 0x0FFFFF).  
  - Può essere incrementata/riconfigurata modificando la costante `MEMORY_SIZE`.

- **ALU** (Arithmetic Logic Unit)  
  - Supporta ADD, SUB, AND, OR, XOR, NOT, SHIFT LEFT, SHIFT RIGHT.  
  - Aggiorna i flag (Zero, Carry, Overflow, Sign).

- **Set di Istruzioni** (ISA)  
  - MOV, MOVI, ADD, SUB, AND, OR, XOR, NOT, SHL, SHR, LOAD, STORE, JMP, JEQ, JNE, PUSH, POP, CALL, RET, DRAW, ecc.  
  - Ogni istruzione è codificata in 32 bit: i primi 8 bit (31..24) sono l’opcode, i restanti campi variano a seconda dell’istruzione.

- **Gestione Grafica**  
  - Uso di SDL2 + OpenGL in modalità 2D ortografica.  
  - L’istruzione `DRAW` disegna un singolo pixel (o un punto) nelle coordinate `(x, y)` con un colore `0xRRGGBB`.

Grazie a questa struttura modulare (CPU, ALU, BUS, MEMORY, IO, GRAPHICS), l’emulatore è facilmente estendibile con nuove istruzioni, interrupt, e feature avanzate.
