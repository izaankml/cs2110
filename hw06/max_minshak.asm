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

ld r0, HEAD_ADDR
brp HEADISNOTNULL
brz HEADISNULL

HEADISNOTNULL
  ldr r1, r0, 1
  ldr r2, r0, 1
  ldr, r0, r0, 0
  brz STOREVALUES

  LOOP
  ldr r3, r0, 1
  not r3, r3
  add r3, r3, 1
  add r4, r1, r3
  brn NEWMAXFOUND
  add r5, r2, r3
  brp NEWMINFOUND
  CONTINUE
  ldr r0, r0, 0
  brnp LOOP
  brz STOREVALUES

NEWMAXFOUND
  not r3, r3
  add r3, r3, 1
  add r2, r3, 0
  br CONTINUE

NEWMINFOUND
  not r3, r3
  add r3, r3, 1
  add r2, r3, 0
  br CONTINUE

HEADISNULL
  ld r1, MIN_INT
  ld r2, MAX_INT
  br STOREVALUES

STOREVALUES
  st r1, ANSWER_MAX
  st r2, ANSWER_MIN
  br END

END
halt


HEAD_ADDR  .fill x4000

MAX_INT    .fill x7FFF
MIN_INT    .fill x8000

ANSWER_MAX .blkw 1
ANSWER_MIN .blkw 1

.end

.orig x4000
  .fill x4002         ;; Node 1
  .fill 4
  .fill x4004         ;; Node 2
  .fill 5
  .fill x4006         ;; Node 3
  .fill 25
  .fill x4008         ;; Node 4
  .fill 1
  .fill x0000         ;; Node 5
  .fill 10
.end
