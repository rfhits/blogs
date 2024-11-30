# strong.asm
add:
        movw    r3, #:lower16:.LANCHOR0
        movt    r3, #:upper16:.LANCHOR0
        dmb     ish
        ldr     r2, [r3]
        dmb     ish
        adds    r1, r2, #1
        dmb     ish
.L2:
        ldrex   r0, [r3]
        cmp     r0, r2
        bne     .L3
        strex   ip, r1, [r3]
        cmp     ip, #0
        bne     .L2
.L3:
        dmb     ish
        bx      lr
count:
        .space  4