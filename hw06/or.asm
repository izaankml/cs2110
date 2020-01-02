;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Izaan Kamal
;;====================================

.orig x3000
            LD R0, A
            LD R1, B

            NOT R0, R0            ;~a
            NOT R1, R1            ;~b
            AND R2, R0, R1        ;ans = ~a & ~b
            NOT R2, R2            ;~ans

            ST R2, ANSWER


            HALT

A      .fill x1010
B      .fill x0404

ANSWER .blkw 1

.end
