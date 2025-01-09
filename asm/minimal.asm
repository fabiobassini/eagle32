; Esempio: disegna un quadrato 10x10 verde in (100..109, 100..109)
        MOVI  R0, 100       ; Inizializza x = 100
quad_x:
        MOVI  R1, 100       ; Inizializza y = 100
quad_y:
        MOVI  R2, 0x00FF00  ; Carica il colore verde in R2
        DRAW  R0, R1, R2    ; Disegna un pixel verde in (x, y)

        ADD   R1, R1, 1     ; Incrementa y
        SUB   R3, R1, 110   ; Calcola R3 = y - 110
        JEQ   inc_x         ; Se y == 110, salta a inc_x
        JMP   quad_y        ; Altrimenti continua a disegnare sulla stessa colonna

inc_x:
        ADD   R0, R0, 1     ; Incrementa x
        SUB   R4, R0, 110   ; Calcola R4 = x - 110
        JEQ   done          ; Se x == 110, abbiamo finito di disegnare
        JMP   quad_x        ; Altrimenti ricomincia con la nuova colonna

done:
        ; Stop: ferma la CPU con una istruzione OP_UNKNOWN
        .db 0xFF, 0, 0, 0
