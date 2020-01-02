;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Izaan Kamal
;;====================================

.orig x3000
            AND R0, R0, 0           ;i = 0
            AND R4, R4, 0           ;answer = 0

WHILE1      LD R1, STR_ADDR_1
            LD R2, STR_ADDR_2
            ADD R1, R0, R1          ;if string1[i] == 0
            LDR R1, R1, 0
            BRZ POTENTIAL

            ADD R2, R0, R2         ;if string2[i] == 0
            LDR R2, R2, 0
            BRZ BIGGER

            NOT R2, R2             ;if string1[i] == string2[i]
            ADD R2, R2, #1
            ADD R3, R2, R1
            BRN SMALLER
            BRP BIGGER

            ADD R0, R0, 1          ;i++
            BR WHILE1


POTENTIAL   ADD R2, R0, R2         ;if string1[i] == 0
            LDR R2, R2, 0
            BRZ DONE

SMALLER     ADD R4, R4, #-1
            BR DONE

BIGGER      ADD R4, R4, 1

DONE        ST R4, ANSWER
            HALT




STR_ADDR_1 .fill x4000
STR_ADDR_2 .fill x4050

ANSWER     .blkw 1

.end

.orig x4000
  .stringz "This is a test"
.end

.orig x4050
  .stringz "This is a rest"
.end