# ts.s file:
    .global getebp
getebp:
        movl %ebp, %eax
        ret
