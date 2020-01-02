;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Izaan Kamal
;;====================================

.orig x3000
            AND R0, R0, 0   ;int i = 0
            AND R1, R1, 0   ;int len = 0
            AND R2, R2, 0
            ST R2, ANSWER

FOR1        LD R1, ARRAY_LEN ;i - len < 0
            NOT R1, R1
            ADD R1, R1, 1
            ADD R1, R0, R1
            BRZP ENDF1

;if ARRAY_ADDR[i] < 0
            LD R1, ARRAY_ADDR
            ADD R1, R0, R1
            LDR R1, R1, 0
            BRZP ENDIF1

; sum += ARRAY_ADDR[i]
            LD R1, ARRAY_ADDR
            ADD R1, R0, R1
            LDR R1, R1, 0
            LD R2, ANSWER
            ADD R2, R2, R1
            ST R2, ANSWER

; reinit
ENDIF1      ADD R0, R0, 1

            BR FOR1
ENDF1       NOP

            HALT




ARRAY_ADDR .fill x4000
ARRAY_LEN  .fill 10

ANSWER     .blkw 1

.end

.orig x4000
  .fill 7
  .fill -18
  .fill 0
  .fill 5
  .fill -9
  .fill 25
  .fill 1
  .fill -2
  .fill 10
  .fill -6
.end
