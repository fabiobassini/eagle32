; =============================================================================
; circle.asm
; Disegna un cerchio “vuoto” (anello) di raggio 50 in (320,240),
; colore rosso (0xFF0000). Usa i salti JEQ e subroutine "mul".
; =============================================================================

        ; REGISTRI USATI
        ; R3 = cx   (centro X)
        ; R4 = cy   (centro Y)
        ; R5 = r    (raggio)
        ; R6 = color
        ; R7 = r^2
        ; R8..R9..R10..R11..R12..R13..R14..R15..R16 usati come vari
        ;
        ; Attenzione: con soli JEQ/JNE possiamo testare solo "dist^2 == r^2"
        ; => disegna un anello. Non un cerchio pieno.

        MOVI  R3, 320        ; cx
        MOVI  R4, 240        ; cy
        MOVI  R5, 50         ; r = 10
        MOVI  R6, 0xFF0000   ; color = rosso (0xRRGGBB)

; ---------------------------------------------------------------------
; 1) calcola r^2 e mettilo in R7
;    => subroutine mul => R2 = R0 * R1
        MOV   R0, R5
        MOV   R1, R5
        CALL  mul
        MOV   R7, R2         ; r^2 => R7

; ---------------------------------------------------------------------
; 2) bounding box: (cx-r, cx+r) x (cy-r, cy+r)
        SUB   R8,  R3, R5    ; startX = cx - r
        ADD   R9,  R3, R5    ; endX   = cx + r

        SUB   R10, R4, R5    ; startY = cy - r
        ADD   R11, R4, R5    ; endY   = cy + r

; x = startX
        MOV   R12, R8

x_loop:
; ---------------------------------------------------------------------
; se x == endX+1 => fine
; costruisci endX+1 in R14, poi x - (endX+1) => se eq => done
        MOVI  R13, 1
        ADD   R14, R9, R13     ; endX + 1
        SUB   R15, R12, R14
        JEQ   done

; y = startY
        MOV   R16, R10

y_loop:
; se y == endY+1 => saltiamo a inc_x
        ADD   R14, R11, R13    ; endY + 1
        SUB   R15, R16, R14
        JEQ   inc_x

; ---------------------------------------------------------------------
; 3) calcola dist^2 = (x-cx)^2 + (y-cy)^2
        SUB   R0, R12, R3      ; (x - cx) in R0
        MOV   R1, R0
        CALL  mul
        MOV   R13, R2          ; xDiff^2 => R13

        SUB   R0, R16, R4      ; (y - cy) in R0
        MOV   R1, R0
        CALL  mul
        ADD   R13, R13, R2     ; dist^2 => R13

; ---------------------------------------------------------------------
; 4) se dist^2 == r^2 => disegna pixel con DRAW x, y, color
;    => sub R15= dist^2, r^2 => se eq => do_draw
        SUB   R15, R13, R7
        JEQ   do_draw

no_draw:
        ; incrementa y
        ADD   R16, R16, 1
        JMP   y_loop

do_draw:
        DRAW  R12, R16, R6
        ; incrementa y
        ADD   R16, R16, 1
        JMP   y_loop

inc_x:
        ADD   R12, R12, 1
        JMP   x_loop

done:
; ---------------------------------------------------------------------
; ferma la CPU => OP_UNKNOWN = 0xFF
        .db 0xFF, 0x00, 0x00, 0x00

; =============================================================================
; SUBROUTINE: mul
; Calcola R2 = R0 * R1 (aritmetica senza segno, no overflow check)
; -----------------------------------------------------------------------------
mul:
        PUSH R2         ; salva R2
        MOVI R2, 0      ; accumulatore = 0

mul_loop:
        ; if (R1 == 0) => fine
        SUB R14, R1, 0
        JEQ mul_end

        ADD R2, R2, R0
        SUB R1, R1, 1
        JMP mul_loop

mul_end:
        POP R14         ; scarta contenuto
        RET
