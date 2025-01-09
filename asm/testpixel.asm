; testpixel.asm
; Disegna un singolo pixel ROSSO (0xFF0000) a (100,100)

        MOVI R0, 100      ; x=100
        MOVI R1, 100      ; y=100
        MOVI R2, 0x0000FF ; color = rosso (0xRRGGBB)
        DRAW R0, R1, R2   ; disegna

        ; Ferma la CPU con OP_UNKNOWN (0xFF)
        .db 0xFF, 0x00, 0x00, 0x00
