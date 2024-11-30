# weak.asm
add:
        movw    r3, #:lower16:.LANCHOR0
        movt    r3, #:upper16:.LANCHOR0
        dmb     ish
        ldr     r2, [r3]
        dmb     ish
        adds    r1, r2, #1
        dmb     ish
        ldrex   r0, [r3]
        cmp     r0, r2
        bne     .L2
        strex   ip, r1, [r3]
.L2:
        dmb     ish
        bx      lr
count:
        .space  4