.orig x3000

HALT

fibonacci
;assume r6 already has stack pointer
;assume r7 has return address
ADD R6, R6, -4 ; place stack pointer to build up first portion of the stack
STR R7, R6, 2 ; save old return address
STR R5, R6, 1 ; sae old frame pointer
ADD R5, R6, 0 ; set current frame pointer
ADD R6, R6, -5 ; place stack pointer to save all registers onto stack
STR R0, R6, 4 ; store R0
; store R1
; etc, add logic of fibonacci
; at end of stack, tear down stack and do all operations in reverse (eg. STR -> LDR)

STACK .fill xF000
.end
