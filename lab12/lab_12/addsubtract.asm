;;===============================
;; Name: Izaan Kamal
;;===============================

.orig x3000

AND R0, R0, 0
AND R1, R1, 0
ADD R1, R1, LENGTH

LOOP
AND
HALT


ARRAY .fill x6000
LENGTH .fill 7
RESULT .fill 0
.end

.orig x6000
.fill 3 ; +
.fill 5 ; -
.fill 1 ; +
.fill 4 ; -
.fill 6 ; +
.fill 8 ; -
.fill 12; +
.end
