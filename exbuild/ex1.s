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
    sub $0, %rsp
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
    mov $0, %rbx
    # test2 = test*5000;
    mov $0, %rbx
    # test2 = test%250;
    mov $0, %rbx
    void_main_u64_u32__epilogue:
    add $0, %rsp
    ret

# void EFIentry(u64,u32)
void_EFIentry_u64_u32:
    sub $0, %rsp
    # a is stored in rdi
    # b is stored in rsi
    void_EFIentry_u64_u32__epilogue:
    add $0, %rsp
    ret

# u64 largeFunc(u64,u64,u64,u256)
u64_largeFunc_u64_u64_u64_u256:
    sub $80, %rsp
    # a is stored in rdi
    # b is stored in rsi
    # c is stored in rdx
    # d is stored at SP+0
    # i is stored in rax
    # _1 is stored in rcx
    # _2 is stored in r8
    # _3 is stored in r9
    # _4 is stored in r10
    # _5 is stored in r11
    # _6 is stored in rbx
    # _7 is stored in rsp
    # _8 is stored in rbp
    # _9 is stored in r12
    # _10 is stored in r13
    # _11 is stored in r14
    # _12 is stored in r15
    # _13 is stored at SP+32
    # _14 is stored at SP+40
    # _15 is stored at SP+48
    # _16 is stored at SP+56
    # _17 is stored at SP+64
    u64_largeFunc_u64_u64_u64_u256__epilogue:
    add $80, %rsp
    ret

# u64 malloc(u64)
u64_malloc_u64:
    sub $0, %rsp
    # size is stored in rdi
    # nsize is stored in rax
    u64_malloc_u64__epilogue:
    add $0, %rsp
    ret

# u64 calloc(u64)
u64_calloc_u64:
    sub $0, %rsp
    # size is stored in rdi
    # ksize is stored in rax
    # ksize = 32KiB;
    mov $32768, %rax
    # ksize = 64KiB;
    mov $65536, %rax
    u64_calloc_u64__epilogue:
    add $0, %rsp
    ret
