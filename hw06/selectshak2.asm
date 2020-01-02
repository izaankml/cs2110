;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Shakeeb Shams
;;====================================

.orig x3000

and r0, r0, 0
and r1, r1, 0
and r2, r2, 0
and r3, r3, 0
and r4, r4, 0
and r5, r5, 0
and r6, r6, 0
and r7, r7, 0

ld r0, ARRAY_LEN
add r5, r0, 0
add r0, r0, -1
ld r1, ARRAY_ADDR
ldi r3, ARRAY_ADDR
add r2, r1, 1


OUTERLOOP

  add r4, r1, 0

  INNERLOOP
    ldr r6, r2, 0
    ldr r7, r4, 0
    not r6, r6
    add r6, r6, 1
    add r7, r6, r7
    brp COPYADDRESS
    CONTINUE
    add r2, r2, 1
    add r5, r5, -1
    brp INNERLOOP


   ldr r6, r4, 0
   str r3, r4, 0
   str r6, r1, 0


  add r1, r1, 0
  ldr r3, r1, 0
  add r2, r1, 1
  add r5, r0, 0
  add r0, r0, -1
  brp OUTERLOOP


br END

COPYADDRESS
add r4, r2, 0
br CONTINUE

END
halt

ARRAY_ADDR .fill x4000
ARRAY_LEN  .fill 10

.end

.orig x4000
  .fill 1
  .fill 16
  .fill -2
  .fill -11
  .fill 7
  .fill 8
  .fill 20
  .fill -4
  .fill 9
  .fill 10
.end
