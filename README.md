# Emulatore CPU 32-bit con Assembler e Grafica OpenGL

<div align="center">
  <img src="assets/eagle32.webp" alt="Header" width="350"/>
</div>

Questo progetto è un emulatore di una **CPU a 32 bit** sviluppato in C, che include:

- Un set di istruzioni esteso (ad esempio: `MOV`, `MOVI`, `ADD`, `SUB`, `AND`, `OR`, `XOR`, `NOT`, `SHL`, `SHR`, `LOAD`, `STORE`, `JMP`, `JEQ`, `JNE`, `PUSH`, `POP`, `CALL`, `RET`, `DRAW`)  
- Una ALU con corretta **gestione dei flag** (Zero, Carry/Borrow, Sign, Overflow)  
- Un’unità di controllo (Control Unit) e un **insieme di registri** (general-purpose)  
- Un **bus** unificato per memoria e I/O  
- **SDL2 + OpenGL** per il **rendering 2D** (l’istruzione `DRAW` consente di disegnare pixel in una finestra)  
- Un **Assembler** (separato in un proprio eseguibile) che traduce il codice assembly in un file `.bin` eseguibile dall’emulatore

## Caratteristiche Principali

1. **CPU Emulata**  
   - Set di istruzioni base ampliato, con supporto per istruzioni aritmetiche, logiche, salti condizionali e subroutine.  
   - Pipeline semplice: fetch, decode, execute.

2. **Memoria e Bus**  
   - Simulazione di una memoria lineare (fino a 1 MB) e di un bus per comunicare con I/O e dispositivi esterni.  

3. **I/O e Interrupt (Futuro)**  
   - Al momento, funzioni di input/output basilari (`io_read`, `io_write`).  
   - Possibile estensione futura per gestire interrupt e periferiche più avanzate.

4. **Grafica**  
   - Usa SDL2 e OpenGL per disegnare punti (pixel) in modalità 2D ortografica.  
   - L’istruzione `DRAW R_x, R_y, R_color` consente di disegnare un singolo pixel, aggiornato in tempo reale.

5. **Assembler**  
   - Supporta la conversione di un sottoinsieme di istruzioni assembly in codice macchina (`.bin`).  
   - Gestisce label, direttive (`.db`) e due pass (primo pass per label, secondo per la generazione binaria).

## Esempio: Disegnare un “cerchio vuoto” (anello)

Nel repository troverai un esempio di codice assembly (`circle.asm`) che disegna un anello rosso al centro dello schermo, sfruttando istruzioni come `CALL mul`, `JEQ`, e `DRAW`.  
Compilando tale sorgente con l’assembler, otterrai un `.bin` che, caricato nell’emulatore, mostrerà il disegno.

## Come Compilare

1. **Emulatore**:  
   - Usa il tuo build system (ad es. `make`) per generare l’eseguibile `emulator`.  
2. **Assembler**:  
   - Allo stesso modo, compila il progetto `assembler` per ottenere un eseguibile `assembler`.  
3. **Esecuzione**:  
   - Assembla un file assembly in un file `.bin`:  
     ```bash
     ./assembler myprog.asm myprog.bin
     ```
   - Esegui nell’emulatore:  
     ```bash
     ./emulator myprog.bin
     ```

## Stato Attuale

- **Set di istruzioni**: completato con istruzioni di base + condizionali + subroutine + `DRAW`.  
- **ALU con Flag**: implementata gestione dei flag (Zero, Carry/Borrow, Sign, Overflow).  
- **Assembler**: funzionante, con due pass (label + generazione). Supporta `.db` per dati raw.  
- **Grafica**: disegno di pixel, con esempi di base (es. disegno di linee, cerchi/anelli).  

## Possibili Estensioni Future

- **Interrupt hardware** e gestione di timer/I/O asincroni  
- **Modalità debug** con disassemblatore integrato e breakpoint  
- **Pipeline più complessa** (stall, forwarding, ecc.)  
- **Caching, paging** e protezione memoria per un vero micro-OS didattico

---

Speriamo che questo progetto possa essere un’ottima base didattica e un trampolino di lancio per ulteriori sperimentazioni nel mondo dell’emulazione e dei sistemi low-level!
