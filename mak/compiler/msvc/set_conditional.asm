    SECTION .text
    global _set_conditional_128
_set_conditional_128:
        mov         r10,    rbx
        mov         rax,    r8
        mov         rbx,    r9
        mov         r8,     rcx
        mov         rcx,    rdx
        mov         rdx,    r9
        inc         rbx
        lock cmpxchg16b [r8]
        setz        al
        mov         rbx,    r10
        ret
