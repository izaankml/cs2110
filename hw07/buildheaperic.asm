;;====================================
;; CS 2110 - Fall 2018
;; Homework 7
;; buildheap.asm
;;====================================
;; Name: Eric Zhu
;;====================================

; Little reminder from your pals: don't run this directly by pressing
; ``Run'' in complx, since (look below) there's nothing put at address
; x3000. Instead, load it and use ``Debug'' -> ``Simulate Subroutine
; Call'' and choose the ``strlen'' label.

.orig x3000
halt

heapify
    ADD R6, R6, -3   ;initialize space for everything
    STR R7, R6, 1    ;store RA
    STR R5, R6, 0    ;store old FP
    ADD R6, R6, -8   ;initialize space for registers
    ADD R5, R6, 7    ;place new FP
                     ;MEM[R5] is largest
                     ;MEM[R5 - 1] is leftChild
                     ;MEM[R5 - 2] is rightChild
    STR R0, R6, 4    ;store R0 on the stack
    STR R1, R6, 3    ;store R1 on the stack
    STR R2, R6, 2    ;store R2 on the stack
    STR R3, R6, 1    ;store R3 on the stack
    STR R4, R6, 0    ;store R4 on the stack

    LDR R0, R5, 4    ;load the start of the array into R0
    LDR R1, R5, 5    ;load n into R1
    LDR R2, R5, 6    ;load i into R2
    STR R2, R5, 0    ;set MEM[R5] to be the index of the largest
    ADD R2, R2, R2
    ADD R2, R2, 1    ;calculate index of leftChild
    STR R2, R5, -1
    ADD R2, R2, 1    ;calculate index of rightChild
    STR R2, R5, -2
    LDR R2, R5, 6
    
    
    LDR R3, R5, -1   ;load index of leftChild
    NOT R3, R3
    ADD R3, R3, 1
    ADD R3, R3, R1
    BRnz TEARDOWN
    
    
    LDR R3, R5, -1   ;load index of leftChild
    ADD R3, R3, R0
    LDR R3, R3, 0    ;load value of leftChild
    NOT R3, R3
    ADD R3, R3, 1
    LDR R4, R5, 0    ;index of largest
    ADD R4, R4, R0
    LDR R4, R4, 0    ;value of largest
    ADD R4, R4, R3   ;arr[largest] - arr[leftChild]
    BRzp NEXT
    LDR R3, R5, -1   ;load index of leftChild
    STR R3, R5, 0    
    
NEXT
    LDR R3, R5, -2   ;load index of rightChild
    NOT R3, R3
    ADD R3, R3, 1
    ADD R3, R3, R1
    BRnz DONE
    
    LDR R3, R5, -2   ;load index of rightChild
    ADD R3, R3, R0
    LDR R3, R3, 0    ;load value of rightChild
    NOT R3, R3
    ADD R3, R3, 1
    LDR R4, R5, 0    ;index of largest
    ADD R4, R4, R0
    LDR R4, R4, 0    ;value of largest
    ADD R4, R4, R3   ;arr[largest] - arr[rightChild]
    BRzp DONE
    LDR R3, R5, -2   ;load index of rightChild
    STR R3, R5, 0

DONE
    LDR R4, R5, 0    ;index of largest
    NOT R4, R4
    ADD R4, R4, 1
    ADD R4, R4, R2
    BRz TEARDOWN


    LDR R4, R5, 0    ;index of largest
    ADD R4, R4, R0   
    LDR R3, R4, 0    ;value of largest
    ADD R2, R2, R0
    LDR R1, R2, 0    ;value at index i of array
    STR R1, R4, 0    
    STR R3, R2, 0   

    
    LDR R0, R5, 4    ;load the start of the array into R0
    LDR R1, R5, 5    ;load n into R1
    ADD R6, R6, -3
    LDR R4, R5, 0    ;index of largest    
    STR R0, R6, 0
    STR R1, R6, 1
    STR R4, R6, 2
    JSR heapify

    ADD R6, R6, 4

TEARDOWN
    LDR R4, R6, 0   ;reload R4
    LDR R3, R6, 1   ;reload R3
    LDR R2, R6, 2   ;reload R2
    LDR R1, R6, 3   ;reload R1
    LDR R0, R6, 4   ;reload R0
    ADD R6, R6, 8   ;add the stack pointer
    LDR R5, R6, 0   ;reload old FP
    LDR R7, R6, 1   ;reload return address
    ADD R6, R6, 2   ;put the stack pointer at the rv
    RET
; [ implement heapify here ]


buildheap
    ADD R6, R6, -3   ;initialize space for everything
    STR R7, R6, 1    ;store RA
    STR R5, R6, 0    ;store old FP
    ADD R6, R6, -4   ;initialize space for registers
    ADD R5, R6, 3    ;place new FP
    STR R0, R6, 3    ;store R0 on the stack
    STR R1, R6, 2    ;store R1 on the stack
    STR R2, R6, 1    ;store R2 on the stack
    STR R3, R6, 0    ;store R3 on the stack

    LDR R0, R5, 4    ;load the address of array into R0
    LDR R1, R5, 5    ;load n into R1
    ADD R2, R1, 0    ;this is the index
    
LOOP
    BRn TEAR
    ADD R6, R6, -3
    STR R0, R6, 0
    STR R1, R6, 1
    STR R2, R6, 2
    JSR heapify
    ADD R6, R6, 4  

    ADD R2, R2, -1
    BR LOOP
    
    

TEAR
    LDR R3, R6, 0   ;reload R3
    LDR R2, R6, 1   ;reload R2
    LDR R1, R6, 2   ;reload R1
    LDR R0, R6, 3   ;reload R0
    ADD R6, R6, 4   ;add the stack pointer
    LDR R5, R6, 0   ;reload old FP
    LDR R7, R6, 1   ;reload return address
    ADD R6, R6, 2   ;put the stack pointer at the rv
    RET
; [ implement buildheap here ]


; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end

; You should not have to LD from any label, take the
; address off the stack instead :)
