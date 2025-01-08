# Architettura della CPU Emulata

Questa documentazione descrive in dettaglio l'architettura modulare dell'emulatore, evidenziando i principali componenti e il loro funzionamento integrato.

## Panoramica dei Componenti

- **CPU Core**: Il cuore dell'emulatore, comprende registri, Program Counter (PC), Stack Pointer (SP), e Flags. Coordina il ciclo di fetch-decode-execute.
- **ALU (Arithmetic Logic Unit)**: Modulo che esegue operazioni aritmetiche (ADD, SUB) e logiche (AND, OR, XOR). Riceve segnali di controllo dalla Control Unit.
- **Unità di Controllo**: Decodifica le istruzioni, determina quali operazioni effettuare e gestisce il flusso di dati tra i componenti.
- **Memoria**: Memoria principale in cui risiedono istruzioni e dati. Funziona con letture/scritture a 32 bit.
- **Bus**: Interconnessione tra CPU, memoria e dispositivi I/O. Il bus gestisce l'accesso alla memoria e ai dispositivi, indirizzando le richieste correttamente.
- **I/O**: Modulo di Input/Output per operazioni su porte I/O simulate, come letture e scritture di dati.
- **Grafica**: Sistema di rendering che utilizza SDL per disegnare pixel su una finestra, implementando l'istruzione DRAW.

## Flusso di Esecuzione

1. **Fetch**: La CPU preleva un'istruzione dalla memoria utilizzando il Bus. Il Program Counter (PC) indica l'indirizzo corrente.
2. **Decode**: L'Unità di Controllo analizza l'istruzione per determinare:
   - Il formato dell'istruzione (R, I, J).
   - Quali registri e/o immediati sono coinvolti.
   - Qual è l'operazione richiesta.
3. **Execute**:
   - Se l'istruzione è aritmetica/logica, la CPU delega all'ALU l'esecuzione dell'operazione.
   - Se è un'operazione di memoria, utilizza il Bus per leggere o scrivere dati.
   - Per salti, modifica il PC.
   - Per operazioni grafiche, invoca le funzioni del modulo grafico.
   - Per I/O, chiama le funzioni specifiche del modulo I/O.
4. **Write-back**: I risultati dell'esecuzione vengono scritti nei registri o nella memoria, aggiornando lo stato della CPU e dei dati.

## Descrizione dei Moduli

- **CPU Core**: Implementato in `cpu/cpu.c`, gestisce il ciclo di esecuzione e interagisce con gli altri moduli tramite chiamate a funzioni.
- **ALU**: In `cpu/alu.c`, riceve comandi dalla Control Unit e restituisce risultati, aggiornando eventualmente i flags.
- **Unità di Controllo**: In `cpu/control_unit.c`, interpreta gli opcode e traduce in segnali di controllo (enumerazione ControlSignal).
- **Memoria**: Fornisce accesso in lettura/scrittura a 32 bit in `memory/memory.c`.
- **Bus**: Gestisce la comunicazione tra CPU, memoria e I/O, definito in `bus/bus.c`.
- **I/O**: Simula operazioni di input/output in `io/io.c`.
- **Grafica**: In `graphics/graphics.c`, usa SDL per disegnare pixel e aggiornare lo schermo.

Questa architettura modulare permette di estendere il sistema aggiungendo nuove istruzioni, dispositivi o migliorando le performance senza modificare l'intero codice base.

