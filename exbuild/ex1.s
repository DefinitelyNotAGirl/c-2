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
    # test is stored in rdi
    # arg is stored in rsi
    # test is stored in rcx
    # test2 is stored in rbx
    # test3 is stored at SP+12
    # rsp is stored in rsp
    # belowStack is stored at SP-8
    # EFER is stored in EFER
    # test2 = test+500;
    mov $500, %rbx
    # test2 = test-100;
    mov $18446744073709551516, %rbx
    # test2 = test / 999;
    xor %rbx, %rbx
    # test2 = test*5000;
    xor %rbx, %rbx
    # test2 = test%250;
    xor %rbx, %rbx
    # test %= 250;
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
    # a is stored in rdi
    # b is stored in rsi
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
    # a is stored in rdi
    # b is stored in rsi
    # c is stored in rdx
    # d is stored at SP+0
    # i is stored in rcx
    # _1 is stored in r8
    # _2 is stored in r9
    # _3 is stored in r10
    # _4 is stored in r11
    # _5 is stored in rbx
    # _6 is stored in rsp
    # _7 is stored in rbp
    # _8 is stored in r12
    # _9 is stored in r13
    # _10 is stored in r14
    # _11 is stored in r15
    # _12 is stored in rax
    # _13 is stored at SP+32
    # _14 is stored at SP+40
    # _15 is stored at SP+48
    # _16 is stored at SP+56
    # _17 is stored at SP+64
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
    # size is stored in rdi
    # nsize is stored in rsi
    u64_malloc_u64__epilogue:
    ret

# u64 calloc(u64)
u64_calloc_u64:
    # size is stored in rdi
    # ksize is stored in rsi
    # ksize = 32KiB;
    mov $32768, %rsi
    # ksize = 64KiB;
    mov $65536, %rsi
    u64_calloc_u64__epilogue:
    ret
