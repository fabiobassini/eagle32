# Guida all'Uso dell'Emulatore

Questa guida spiega passo passo il funzionamento interno dell'emulatore, descrivendo il ruolo delle funzioni principali e il flusso di esecuzione.

## Ciclo Principale dell'Emulatore

1. **Inizializzazione**:
   - `memory_init()`: Inizializza la memoria allocando e azzerando l'array di memoria.
   - `io_init()`: Prepara il sistema di I/O (se necessario).
   - `graphics_init()`: Inizializza SDL e prepara la finestra grafica.

2. **Configurazione della CPU**:
   - Si crea un'istanza di `CPU` e si chiama `cpu_init(&cpu)`, che a sua volta chiama `cpu_reset(&cpu)`.
   - `cpu_reset()` azzera i registri, imposta il Program Counter e altri registri di stato.

3. **Caricamento del Programma**:
   - Le istruzioni vengono scritte nella memoria utilizzando `bus_write_word(indirizzo, istruzione)`.
   - Nel file `main.c`, viene caricata un'istruzione DRAW di esempio e impostati i registri corrispondenti.

4. **Esecuzione**:
   - `cpu_execute(&cpu)` avvia il ciclo fetch-decode-execute.
   - All'interno di `cpu_execute()`:
     - **Fetch**: Viene letta un'istruzione dalla memoria tramite `bus_read_word(cpu->pc)`.
     - **Decode**: L'istruzione viene decodificata con `decode_instruction()`, che restituisce un segnale di controllo.
     - **Execute**: In base al segnale:
       - Per operazioni aritmetiche (`ADD`, `SUB`), la CPU chiama `alu_execute()` con gli operandi.
       - Per operazioni di memoria (`LOAD`, `STORE`), la CPU usa funzioni di lettura/scrittura del bus.
       - Per salti (`JMP`), modifica il Program Counter.
       - Per `DRAW`, la funzione `graphics_draw()` viene invocata per disegnare un pixel.
     - Il ciclo continua fino a quando non si incontra un'istruzione non riconosciuta o un'istruzione di terminazione.

5. **Aggiornamento Grafico e Chiusura**:
   - Dopo l'esecuzione, `graphics_update()` aggiorna la finestra SDL.
   - L'emulatore attende (es. con `SDL_Delay()`), quindi chiama `graphics_cleanup()` per rilasciare risorse grafiche.

## Ruolo delle Funzioni Chiave

- **`bus_read_word()` / `bus_write_word()`**: Incapsulano l'accesso alla memoria e ai dispositivi I/O, controllando se un indirizzo appartiene alla memoria principale o a una periferica.
- **`alu_execute()`**: Riceve un'operazione e due operandi, esegue l'operazione aritmetica/logica e restituisce il risultato.
- **`decode_instruction()`**: Analizza l'opcode di un'istruzione per determinare quale operazione la CPU deve eseguire.
- **`graphics_draw()`**: Disegna un pixel sullo schermo alla posizione e colore specificati.

Questa guida fornisce una panoramica del funzionamento interno dell'emulatore, facilitando la comprensione del flusso di esecuzione e l'interazione tra i moduli.

