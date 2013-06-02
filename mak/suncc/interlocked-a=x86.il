.inline fetch_and_add_32, 0
        movd        %edi,       %xmm1
        mov         4(%esp),    %eax
        mov         (%esp),     %edi
 lock;  xadd        %eax,       (%edi)
        movd        %xmm1,      %edi
.end

.inline fetch_and_set_32, 0
        movd        %edi,       %xmm1
        mov         4(%esp),    %eax
        mov         (%esp),     %edi
 lock;  xchg        %eax,       (%edi)
        movd        %xmm1,      %edi
.end

.inline set_conditional_32, 0
        movd        %edi,       %xmm1
        mov         8(%esp),    %eax
        mov         4(%esp),    %ecx
        mov         (%esp),     %edi
 lock;  cmpxchg     %ecx,       (%edi)
        movd        %xmm1,      %edi
.end

.inline set_conditional_64, 0
        movd        %ebx,       %xmm0
        movd        %edi,       %xmm1
        mov         16(%esp),   %edx
        mov         12(%esp),   %eax
        mov         8(%esp),    %ebx
        mov         4(%esp),    %ecx
        mov         (%esp),     %edi
 lock;  cmpxchg8b   (%edi)
        movd        %xmm1,      %edi
        movd        %xmm0,      %ebx
.end
