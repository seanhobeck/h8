# h8

h8 is a side-project that I've been working on during vacation,
and is heavily inspired by jdh's 8-bit emulator. this is entirely just
for fun and for practice.

## instruction set

    1: mov imm16/reg, imm16/reg     ; moving data from or to registers & memory.
    2: lda imm8, imm8/reg		   ; loading a byte of data into a register or memory.
    3: add imm16/reg, imm8/reg	; addition with registers & memory.
    4: sub imm16/reg, imm8/reg	; subtraction with register & memory.
    5: cmp reg, imm16/reg		; comparing a register to memory or another register.
    6: or imm16/reg, imm8/reg	; bitwise 'or' operator for registers & memory.
    7: not imm16/reg, imm8/reg	; bitwise 'not' operator for registers & memory.
    8: and imm16/reg, imm8/reg	; bitwise 'and' operator for registers & memory.
    9: jc flag, imm16/reg   	; jumping under a flagged condition to a instruction.
    A: hlt 				; halting the program
    B: out imm8/reg			; stdout a register or the value in memory.

## architecture

the h8 processor has around 65kb of memory, ranging from [0x0000 - 0x7FFF].
   unlike other 8-bit and modern processors, h8 has direct access to the *%ip (instruction pointer)*, *%ax (return address register)*, *zf (zero flag), cf (carry flag), gf(greater-then flag), lf(less-then flag), and sf(signed-flag)*.
