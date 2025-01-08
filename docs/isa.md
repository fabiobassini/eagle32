# Instruction Set Architecture (ISA)

L'ISA definisce il set di istruzioni e i formati utilizzati dalla CPU emulata. Ogni istruzione è codificata su 32 bit e segue uno dei tre formati principali: R-Type, I-Type e J-Type.

## Formati delle Istruzioni

### R-Type (Register)
Utilizzato per operazioni che coinvolgono esclusivamente registri.
- **Formato:**
  - Bits 31-24: **Opcode** (8 bit) - Identifica l'operazione da eseguire.
  - Bits 23-20: **Rd** (4 bit) - Registro di destinazione.
  - Bits 19-16: **Rs** (4 bit) - Primo operando.
  - Bits 15-12: **Rt** (4 bit) - Secondo operando.
  - Bits 11-0: **Padding** - Non utilizzati, riservati per future espansioni o per allineamento.

- **Esempio:**
  - `ADD R1, R2, R3`
    - Opcode = 0x02 (ADD)
    - Rd = 1, Rs = 2, Rt = 3
    - Codifica binaria (semplificata): `0x02 1 2 3 0x000`

### I-Type (Immediate/Memory)
Utilizzato per istruzioni che necessitano di un valore immediato o di un indirizzo di memoria.
- **Formato:**
  - Bits 31-24: **Opcode** (8 bit)
  - Bits 23-20: **R** (4 bit) - Registro coinvolto (destinazione o sorgente).
  - Bits 19-0: **Immediate/Indirizzo** (20 bit) - Valore immediato o indirizzo di memoria.

- **Esempio:**
  - `LOAD R4, [0x1000]`
    - Opcode = 0x10 (LOAD)
    - R = 4, Immediate = 0x1000
    - Codifica: `0x10 4 0x001000`

### J-Type (Jump)
Utilizzato per istruzioni di salto.
- **Formato:**
  - Bits 31-24: **Opcode** (8 bit)
  - Bits 23-0: **Indirizzo** (24 bit) - Indirizzo di salto.

- **Esempio:**
  - `JMP 0x200`
    - Opcode = 0x20 (JMP)
    - Indirizzo = 0x000200
    - Codifica: `0x20 0x000200`

## Dettagli delle Istruzioni Supportate

- **MOV Rd, Rs** (0x01, R-Type): Copia il valore dal registro Rs in Rd.
- **ADD Rd, Rs, Rt** (0x02, R-Type): Somma i valori nei registri Rs e Rt e memorizza il risultato in Rd.
- **SUB Rd, Rs, Rt** (0x03, R-Type): Sottrae il valore in Rt da quello in Rs e memorizza il risultato in Rd.
- **LOAD Rd, [Imm]** (0x10, I-Type): Legge un valore di 32 bit dalla memoria all'indirizzo specificato da Imm e lo carica in Rd.
- **STORE Rs, [Imm]** (0x11, I-Type): Scrive il contenuto di Rs nella memoria all'indirizzo specificato da Imm.
- **JMP Imm** (0x20, J-Type): Imposta il Program Counter (PC) all'indirizzo specificato da Imm.
- **DRAW Rx, Ry, Rc** (0x30, R-Type): Disegna un pixel alle coordinate (Rx, Ry) con colore Rc.

Questo set di istruzioni è estendibile. Ogni nuova istruzione dovrebbe seguire uno di questi formati e aggiornare la documentazione di conseguenza.

