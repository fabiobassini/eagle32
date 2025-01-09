# Documentazione Assemblatore Modulare

Questo documento descrive in breve il funzionamento dell'assembler modulare:

## Struttura generale

- **scanner.h / scanner.c**: si occupano del "primo livello" di parsing, 
  ovvero tokenizzare le righe di input.  
  - Rimuovono commenti, dividono la riga in (etichetta opzionale, istruzione/direttiva, argomenti).  
- **parser.h / parser.c**: implementano un semplice assembler two-pass.  
  - **Primo pass**: raccoglie le label e l’ordine (indirizzo) delle istruzioni.  
  - **Secondo pass**: risolve i riferimenti a label nei salti e produce il file binario.  
- **main_asm.c**: è il punto di ingresso dell’assembler. Si occupa di:  
  1) Leggere i parametri da riga di comando (file .asm, file .bin in output).  
  2) Invocare la funzione “assemble_file” (o equivalenti) che esegue i due pass.  
  3) Mostrare eventuali errori o messaggi di successo.

## Flusso di Lavoro

1. **Lo scanner** legge riga per riga, genera delle strutture dati “parziali” che contengono:  
   - Eventuale `label:`  
   - Mnemonico (MOV, ADD, ecc.)  
   - Argomenti (R0, R1, numeri, ecc.)  
   - Tipo di direttiva (es. `.db`)  
2. **Il parser** esegue due passate. Nella prima colleziona le label e associa ogni istruzione a un “indirizzo” (inteso come contatore di instruction). Nella seconda sostituisce i riferimenti a label (JMP label) con l’indirizzo numerico.  
3. **Si genera** un file binario (`.bin`) contenente i codici macchina a 32 bit per la CPU personalizzata.

## Aggiunta di nuove istruzioni

Per aggiungere un'istruzione all’ISA:
1. Aggiungere l’opcode in `opcode.h`.
2. Aggiungere i casi corrispondenti nello switch del parser (es. “ADD”, “SUB”, “DRAW”, ecc.).
3. Se necessario, gestire la logica di parsing (numero argomenti, interpretazione immediati, registri, ecc.).

## Error Handling

Questo assemblatore è dimostrativo: gestisce errori minimi (es. se c’è un registro fuori range, stampa un messaggio). In un contesto produttivo, andrebbero gestiti più casi (label inesistenti, errori sintattici, overflow di costanti, ecc.).

## Esempio

Esempio di file `.asm`:

```asm
start:
    MOVI  R0, 10
    MOVI  R1, 20
    ADD   R2, R0, R1
    JMP   end

    .db 0xFF, 0x00, 0x00, 0x00

end:
    OP_UNKNOWN
