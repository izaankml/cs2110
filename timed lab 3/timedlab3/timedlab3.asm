
;;====================================================
;; CS 2110 - Fall 2018
;; Timed Lab 3
;; timedlab3.asm
;;====================================================
;; Name: Izaan Kamal
;;====================================================

.orig x3000

        AND R0, R0, 0               ;i = 0
        AND R4, R4, 0
        LD R2, ASCII_0
        LD R3, ASCII_9

WHILE   LD R1, STR_ADDR             ;while (string[i] != 0)
        ADD R1, R0, R1
        LDR R1, R1, 0
        BRZ DONE

        ADD R4, R1, R2      ;x < 48 && x > 57 -> x - 48 < 0 && x - 57 > 0
        BRN ENDIF

        ADD R4, R1, R3
        BRN ENDIF

        LD R4, ASCII_SPACE
        LD R1, STR_ADDR
        ADD R1, R0, R1
        STR R4, R1, 0

ENDIF   ADD R0, R0, 1
        BR WHILE


DONE    HALT

STR_ADDR    .fill x5000
ASCII_0     .fill -48
ASCII_9     .fill -57
ASCII_SPACE .fill 32
.end

.orig x5000
  .stringz "asdfasdfasdf"
.end