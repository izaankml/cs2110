
;;====================================================
;; CS 2110 - Fall 2018
;; Timed Lab 4
;; converge.asm
;;====================================================
;; Name:
;;====================================================

.orig x3000

;; Don't try to run this code directly, since it only contains
;; subroutines that need to be invoked using the LC-3 calling
;; convention. Use Debug > Setup Test or Simulate Subroutine
;; Call in complx instead.
;;
;; Do not remove this line or you will break...
;; 'Simulate Subroutine Call'

halt

converge
    ADD R6, R6, -3   ;initialize space for everything
    STR R7, R6, 1    ;store RA
    STR R5, R6, 0    ;store old FP
    ADD R6, R6, -5   ;initialize space for registers
    ADD R5, R6, 4    ;place new FP
    STR R0, R6, 4    ;store R0 on the stack
    STR R1, R6, 3    ;store R1 on the stack
    STR R2, R6, 2    ;store R2 on the stack
    STR R3, R6, 1    ;store R3 on the stack
    STR R4, R6, 0    ;store R4 on the stack

    LDR R0, R5, 4   ;load n into R0
    AND R1, R1, 0   ;zero out r1, this will be answer

    ADD R2, R0, -1  ;if (n == 1) return 0;
    BRZ TEARDOWN

    AND R2, R2, 0   ;div = divide(n/2)
    ADD R2, R2, 2   ;
    ADD R6, R6, -2  ;
    STR R0, R6, 0   ;
    STR R2, R6, 1   ;
    LD R4, DIV_ADDR
    JSRR R4         ;

    LDR R2, R6, 0   ;
    ADD R6, R6, 3   ;

    AND R3, R0, 1   ;mod = n % 2

    ADD R3, R3, 0   ;
    BRZ ENDIF1      ;

    ADD R4, R0, R0  ;R4 = n + n = 2n
    ADD R4, R0, R4  ;r4 = 2n + n = 3n
    ADD R4, R4, 1   ;r4 = 3n + 1

    ADD R6, R6, -1
    STR R4, R6, 0
    JSR converge

    LDR R1, R6, 0   ;
    ADD R6, R6, 2   ;
    ADD R1, R1, 1   ;
    BR TEARDOWN

ENDIF1
    ADD R6, R6, -1
    STR R2, R6, 0
    JSR converge

    LDR R1, R6, 0
    ADD R6, R6, 2
    ADD R1, R1, 1



TEARDOWN
    STR R1, R5, 3
    LDR R4, R6, 0
    LDR R3, R6, 1   ;reload R3
    LDR R2, R6, 2   ;reload R2
    LDR R1, R6, 3   ;reload R1
    LDR R0, R6, 4   ;reload R0
    ADD R6, R6, 5   ;add the stack pointer
    LDR R5, R6, 0   ;reload old FP
    LDR R7, R6, 1   ;reload return address
    ADD R6, R6, 2   ;put the stack pointer at the rv
    RET

STACK    .fill xF000
DIV_ADDR .fill x6000 ;; Call the divide subroutine at
                     ;; this address!

.end

;;====================================================
;;   divide(n, d): Takes a numerator (n) and
;;                 denominator (d), returning n / d.
;;====================================================
.orig x6000
divide  ;; DO NOT call JSR with this label! Use DIV_ADDR instead!
  .fill x1DBD
  .fill x7F81
  .fill x7B80
  .fill x1BBF
  .fill x1DBB
  .fill x7140
  .fill x737F
  .fill x757E
  .fill x777D
  .fill x797C
  .fill x6144
  .fill x6345
  .fill x54A0
  .fill x1620
  .fill x987F
  .fill x1921
  .fill x1903
  .fill x0805
  .fill x14A1
  .fill x987F
  .fill x1921
  .fill x16C4
  .fill x0FF7
  .fill x7543
  .fill x6140
  .fill x637F
  .fill x657E
  .fill x677D
  .fill x697C
  .fill x1D61
  .fill x6B80
  .fill x1DA1
  .fill x6F80
  .fill x1DA1
  .fill xC1C0
.end
