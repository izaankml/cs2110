;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Izaan Kamal
;;====================================

.orig x3000
            LD R7, ARRAY_LEN
            NOT R7, R7
            ADD R7, R7, 1
            AND R0, R0, 0

ICOND       ADD R2, R0, R7
            BRN LOOP1
            BRZP ENDLOOP1

LOOP1       AND R5, R0, R0
            LDI R3, ARRAY_ADDR
            ADD R1, R0, 1

JCOND       ADD R2, R1, R7
            BRN LOOP2
            BRZP ENDLOOP2

LOOP2       NOT R2, R0
            ADD R2, R2, 1
            ADD R2, R1, R2
            LD R6, ARRAY_ADDR
            ADD R2, R2, R6
            LDR R4, R2, 0
            NOT R2, R4
            ADD R2, R2, 1
            ADD R2, R3, R2
            BRNZ NOMINIDX
            AND R5, R1, R1
            AND R3, R4, R4
NOMINIDX    ADD R1, R1, 1
            BR  JCOND

ENDLOOP2    NOT R1, R0
            ADD R1, R1, 1
            ADD R1, R5, R1
            BRZ SWAPLESS
            LDI R2, ARRAY_ADDR
            STI R3, ARRAY_ADDR
            ADD R1, R1, R6
            STR R2, R1, 0

SWAPLESS    LD R1, ARRAY_ADDR
            ADD R1, R1, 1
            ST R1, ARRAY_ADDR
            ADD R0, R0, 1
            BR ICOND

ENDLOOP1    LD R0, ARRAY_ORIG
            ST R0, ARRAY_ADDR
            HALT


ARRAY_ORIG .fill x4000
ARRAY_ADDR .fill x4000
ARRAY_LEN  .fill 10

.end

.orig x4000
  .fill 7
  .fill 18
  .fill 0
  .fill 5
  .fill 9
  .fill 25
  .fill 1
  .fill 2
  .fill 10
  .fill 6
.end
