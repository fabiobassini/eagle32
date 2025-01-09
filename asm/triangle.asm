; ============================================================================
; triangle.asm
; Disegna un triangolo "wireframe" verde su schermo:
;   A(100,100), B(300,100), C(200,200)
; usando 3 chiamate a drawLine(A,B), drawLine(B,C), drawLine(C,A).
;
; Dipendenze:
;   - subroutine mul (uguale a quella vista nel cerchio, es. "R2 = R0 * R1")
;   - subroutine div (divisione intera non segno, "R2 = R0 / R1")
;   - subroutine drawLine (riportata qui sotto in forma naive).
;
; Attenzione: se la CPU non implementa la divisione, sostituire con Bresenham.
; ============================================================================
        ; R0..R4, R5.. etc usati nelle subroutines
        ; R6 conterrà il colore
        ; R2 = accumulatore di mul/div

        MOVI  R6, 0x00FF00   ; colore verde = 0x00FF00 (RRGGBB)

;-------------------------------------------------------------------------
; 1) disegna la linea AB: (100,100) -> (300,100)
        MOVI  R0, 100        ; x1
        MOVI  R1, 100        ; y1
        MOVI  R2, 300        ; x2
        MOVI  R3, 100        ; y2
        CALL  drawLine

;-------------------------------------------------------------------------
; 2) disegna la linea BC: (300,100) -> (200,200)
        MOVI  R0, 300
        MOVI  R1, 100
        MOVI  R2, 200
        MOVI  R3, 200
        CALL  drawLine

;-------------------------------------------------------------------------
; 3) disegna la linea CA: (200,200) -> (100,100)
        MOVI  R0, 200
        MOVI  R1, 200
        MOVI  R2, 100
        MOVI  R3, 100
        CALL  drawLine

; infine fermiamo la CPU con OP_UNKNOWN = 0xFF
stop:
        .db 0xFF, 0x00, 0x00, 0x00


; ============================================================================
; SUBROUTINE drawLine:
; Disegna una linea dal punto (x1,y1) al punto (x2,y2) con colore in R6.
; Usa un approccio naive:
;   - Assume x1 <= x2. Se non lo è, scambia i punti.
;   - Calcola dx = x2 - x1, dy = y2 - y1.
;   - Per x da x1 a x2:
;       y = y1 + (dy*(x - x1)) / dx
;       DRAW x, y, R6
;
; Parametri:
;   R0 = x1
;   R1 = y1
;   R2 = x2
;   R3 = y2
;   R6 = colore
;
; Usa subroutines "mul" e "div" (riportate sotto).
; ============================================================================
drawLine:
        ; se x2 == x1 => linea verticale? => gestisci caso a parte
        SUB  R10, R2, R0       ; R10 = (x2 - x1)
        JEQ  vertical_case

        ; se x2 < x1 => swappa (x1,x2) e (y1,y2)
        ; per vedere se (x2 - x1) <0, lo deduciamo se R10 < 0. 
        ; Ma abbiamo solo JEQ e JNE => useremo un trucco...
        ; In un progetto reale, potresti definire un'istruzione "BLT" o "BGT",
        ; oppure un flag sign dopo la SUB. Qui mostriamo come potresti usare un trick:
swap_check:
        ; Abbiamo R10 = x2-x1. Se R10 != 0 => andiamo a sign_check
        MOVI  R11, 0
        SUB   R12, R10, R11    ; R12 = R10 - 0
        JNE   sign_check
        ; se R10 == 0 => salta a vertical_case
        JMP   vertical_case

sign_check:
        ; se (x2-x1) <0 => sign bit => R10 & 0x80000000 => 1
        ; useremmo FLAG_SIGN, ma se non abbiamo, usiamo un test manuale:
        MOVI  R13, 0x80000000
        AND   R14, R10, R13    ; R14 = R10 & 0x80000000
        SUB   R15, R14, 0
        JEQ   skip_swap        ; se R14==0 => (x2-x1) >=0 => skip
        ; se R14!=0 => c'è bit di segno => x2<x1 => swap

        ; scambia x1<->x2
        MOV   R16, R0
        MOV   R0,  R2
        MOV   R2,  R16

        ; scambia y1<->y2
        MOV   R16, R1
        MOV   R1,  R3
        MOV   R3,  R16
skip_swap:

        ; ricalcoliamo dx,dy
        SUB   R10, R2, R0   ; dx
vertical_case:
        SUB   R11, R3, R1   ; dy

        ; se dx == 0 => retta verticale pura => disegniamo con un loop su y
        SUB   R15, R10, 0
        JEQ   line_vertical

        ; memorizziamo x1,y1 in R8,R9 come "start"
        MOV   R8, R0
        MOV   R9, R1

        ; dx => R10, dy => R11
        ; loop: x va da x1..x2 => x - x1 in R17
        ; y = y1 + (dy*(x-x1))/dx

line_loop:
        ; se x> x2 => fine
        SUB  R12, R8, R2
        ; se R12 ==0 => X= x2 => disegniamo l'ultimo punto e fine
        JEQ  line_last
        ; se R8> x2 => esci
        ; (stesso problema di sign: se R12>0 => skip, ecc.)
        ; per brevità, assumiamo x2 >= x1 e x++ => ci fermeremo col JEQ
        ; e un check successivo se x2-x= negative => done

        ; calcola xRel = (x - x1)
        SUB   R17, R8, R0
        ; tmp = dy*xRel => subroutine mul => R2= result
        MOV   R0, R11
        MOV   R1, R17
        CALL  mul
        MOV   R18, R2

        ; yOff = tmp / dx => subroutine div => R2= result
        MOV   R0, R18
        MOV   R1, R10
        CALL  div
        MOV   R19, R2

        ; y = y1 + yOff
        ADD   R19, R19, R1     ; <-- attento: R1 adesso è dx, 
                               ; serve y1? memorizzala in R9 all'inizio
        ; Quindi, sostituiamo con "ADD R19, R19, R9"

        ADD   R19, R19, R9

        ; disegna pixel (x,y)
        DRAW  R8, R19, R6

        ; incrementa x => R8++
        ADD   R8, R8, 1
        JMP   line_loop

line_last:
        ; disegna l'ultimo pixel
        ; y= y1 + ...
        SUB   R17, R8, R0
        MOV   R0, R11
        MOV   R1, R17
        CALL  mul
        MOV   R18, R2

        MOV   R0, R18
        MOV   R1, R10
        CALL  div
        MOV   R19, R2
        ADD   R19, R19, R9

        DRAW  R8, R19, R6
        JMP   line_done

line_vertical:
        ; se dx=0, la linea è verticale. x rimane costante= x1, y va da y1..y2
        ; assumiamo y2>y1. Se non lo fosse, potresti swapparle in precedenza
        MOV   R8, R1   ; y= y1
vert_loop:
        ; se y == y2 => disegna ultimo e fine
        SUB   R12, R8, R3
        JEQ   vert_last
        ; disegna pixel (x1, y)
        DRAW  R0, R8, R6
        ADD   R8, R8, 1
        JMP   vert_loop

vert_last:
        DRAW  R0, R8, R6
        JMP   line_done

line_done:
        RET


; ============================================================================
; SUBROUTINE: mul
; R2 = R0 * R1 (aritmetica senza segno, naive: somma ripetuta)
; ----------------------------------------------------------------------------
mul:
        PUSH R2
        MOVI R2, 0   ; accumulatore = 0

mul_loop:
        SUB R14, R1, 0
        JEQ mul_end

        ADD R2, R2, R0
        SUB R1, R1, 1
        JMP mul_loop

mul_end:
        POP R14
        RET


; ============================================================================
; SUBROUTINE: div
; R2 = R0 / R1 (divisione intera senza segno, naive: contatore di quante volte
;              R1 sta in R0). Non gestiamo R1=0 => infinito loop.
; ----------------------------------------------------------------------------
div:
        PUSH R2
        MOVI R2, 0

div_loop:
        ; se (R0 < R1) => fine
        SUB R14, R0, R1
        ; se eq => R0==R1 => 1 in R2 => fine
        JEQ div_eq
        ; se R14<0 => fine => non abbiamo BLT => useremo trick sign. 
        MOVI R15, 0x80000000
        AND  R16, R14, R15
        SUB  R17, R16, 0
        JEQ div_sub_ok   ; se R16=0 => non c'è bit di segno => R14>=0 => ok
        ; se non è zero => R14<0 => fine
        JMP div_end

div_sub_ok:
        ; R0= R14, R2++
        MOV  R0, R14
        ADD  R2, R2, 1
        JMP div_loop

div_eq:
        ; R0==R1 => R2++ e fine
        ADD R2, R2, 1
div_end:
        POP R14
        RET
