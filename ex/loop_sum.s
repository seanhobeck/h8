lda $0, r1
lda $10, r5
add $1, r1
cmp r1, r5
jc lf, $3
hlt