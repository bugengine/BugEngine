    SECTION .text
    global _set_conditional_128
_set_conditional_128:
        mov         rbx,    r10
        mov         r8,     rax
        mov         rcx,    r8
        mov         rdx,    rcx
        mov         r9,     rbx
        mov         r9,     rdx
        inc         rbx
        lock cmpxchg16b [r8]
        setz        al
        mov         r10,    rbx
        ret
