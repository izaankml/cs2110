;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Izaan Kamal
;;====================================

.orig x3000
            AND R0, R0, 0       ; i = 0
            AND R1, R1, 0       ; j = 0
            AND R2, R2, 0       ; len = 0
            ;AND R3, R3, 0       ; min_idx = 0
            AND R4, R4, 0

FOR1        LD R2, ARRAY_LEN    ; i - (len-1) < 0
            ADD R2, R2, #-1
            NOT R2, R2
            ADD R2, R2, 1
            ADD R2, R0, R2
            BRZP ENDF1

            ADD R1, R0, R1
            ADD R1, R1, 1

            ;AND R3, R3, 0
            ;ADD R3, R0, R3
            STR R0, R3, 0

FOR2        LD R2, ARRAY_LEN    ; j = i + 1, j - len < 0
            NOT R2, R2
            ADD R2, R2, 1
            ADD R2, R1, R2
            BRZP ENDF2

            LD R2, ARRAY_ADDR   ; if arr[j] < arr[min_idx]
            ADD R2, R1, R2
            LDR R2, R2, 0
            LD R4, ARRAY_ADDR
            ADD R4, R3, R4
            LDR R4, R4, 0
            ADD R4, R2, R4
            BRZP ENDIF1

            ;AND R3, R3, 0
            ;ADD R3, R1, R3
            STR R1, R3, 0

ENDIF1      ADD R1, R1, 1
            BR FOR2

ENDF2       LD R2, ARRAY_ADDR   ; swap(arr[i], arr[min_idx])
            ADD R2, R0, R2
            LDR R1, R2, 0
            LD R4, ARRAY_ADDR
            ADD R4, R3, R4
            LDR R3, R4, 0
            STR R1, R4, 0
            STR R3, R2, 0
            AND R1, R1, 0
            ADD R0, R0, 1
            BR FOR1

ENDF1      HALT









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
