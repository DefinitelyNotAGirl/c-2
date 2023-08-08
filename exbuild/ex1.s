# cpe2

extern void_operatorADDEQUAL_string
extern u64_malloc_u64
global void_main_u64_u32
global void_EFIentry_u64_u32
global u64_largeFunc_u64_u64_u64_u256
global u64_malloc_u64
global u64_calloc_u64
extern void_zalloc_u64
extern u64_error_u64

.data:

.rodata:

.bss:

.text:

# void main(u64,u32)
void_main_u64_u32:
    mov %rbx, %rsp+$0 # save @0
    mov $500, %rbx
    mov $18446744073709551516, %rbx
    xor %rbx, %rbx
    xor %rbx, %rbx
    xor %rbx, %rbx
    xor %rdx, %rdx
    mov %rcx, %rax
    mov $250, %r8
    div %r8
    mov %rdx, %rcx
    void_main_u64_u32__epilogue:
    mov %rsp+$0, %rbx # restore @0
    add $8, %rsp
    ret

# void EFIentry(u64,u32)
void_EFIentry_u64_u32:
    void_EFIentry_u64_u32__epilogue:
    ret

# u64 largeFunc(u64,u64,u64,u256)
u64_largeFunc_u64_u64_u64_u256:
    sub $80, %rsp
    mov %rbx, %rsp-$8 # save @1
    mov %rbp, %rsp-$16 # save @2
    mov %r12, %rsp-$24 # save @3
    mov %r13, %rsp-$32 # save @4
    mov %r14, %rsp-$40 # save @5
    mov %r15, %rsp-$48 # save @6
    u64_largeFunc_u64_u64_u64_u256__epilogue:
    mov %rsp-$8, %rbx # restore @1
    mov %rsp-$16, %rbp # restore @2
    mov %rsp-$24, %r12 # restore @3
    mov %rsp-$32, %r13 # restore @4
    mov %rsp-$40, %r14 # restore @5
    mov %rsp-$48, %r15 # restore @6
    add $80, %rsp
    ret

# u64 malloc(u64)
u64_malloc_u64:
    u64_malloc_u64__epilogue:
    ret

# u64 calloc(u64)
u64_calloc_u64:
    mov $32768, %rsi
    mov $65536, %rsi
    u64_calloc_u64__epilogue:
    ret
