.inline fetch_and_add_32, 0
 lock;  xadd        %esi,   (%rdi)
        mov         %esi,   %eax
.end

.inline fetch_and_add_64, 0
 lock;  xaddq       %rsi,   (%rdi)
        movq        %rsi,   %rax
.end

.inline fetch_and_set_32, 0
 lock;  xchg        %esi,   (%rdi)
        mov         %esi,   %eax
.end

.inline fetch_and_set_64, 0
 lock;  xchgq       %rsi,   (%rdi)
        movq        %rsi,   %rax
.end

.inline set_conditional_32, 0
        mov         %edx,   %eax
 lock;  cmpxchg     %esi,   (%rdi)
.end

.inline set_conditional_64, 0
        mov         %rdx,   %rax
 lock;  cmpxchg     %rsi,   (%rdi)
.end

/ dest = rdi
/ newval = rsi
/ oldval = rdx
/ tag = rcx
/ lock;  cmpxchg16b  (%r8)
.inline set_conditional_128, 0
        mov         %rbx,   %r9
        mov         %rdx,   %rax
        mov         %rcx,   %rdx
        mov         %rcx,   %rbx
        mov         %rdi,   %r8
        mov         %rsi,   %rcx
        inc         %rbx
        .byte 0xF0,0x49,0x0F,0xC7,0x08
        setz        %al
        mov         %r9,    %rbx
.end
