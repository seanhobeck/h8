lda $0, r1
lda $1, r2
lda $0, r3
lda $89, r5
add r1, r3
add r2, r3
out r1
out r2
out r3
mov r2, r1
mov r3, r2
lda $0, r3
add r1, r3
add r2, r3
out r3
cmp r3, r5
jc lf, $10
hlt