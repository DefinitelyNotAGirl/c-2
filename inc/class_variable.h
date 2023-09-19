/**
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 17th 2023 9:04:51 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2023 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <common.h>

class type;

/*
architectures:
    0x**********XXXX: arch bits
        0: x86-64 (amd64)
        1: x86-32 (i386)
        2: ARM32
        3: ARM64 (AArch64)
        4-15: reserved (MBZ)
privilege levels:
    0x********00****: cpl 0
    0x********01****: cpl 1
    0x********02****: cpl 2
    0x********03****: cpl 3
register size:
    0x******XX******: XX is the size of the register in bytes
register type:
    0x****00********: standart register (ex. rax)
    0x****01********: x86 model specific register (ex. EFER)
    0x****02********: x86 control register (ex. cr0)
    0x****03********: x86 segment register (ex. cs)
    0x****04********: misc special register (ex. GDTR)
    0x****05********: x86 Floating point register (ex: xmm0)
    0x****06********: x86 debug register (ex. dr0)
*/
#define BITMASK_REGISTER_TYPE 0x0000FF00000000
#define BITMASK_REGISTER_SIZE 0x000000FF000000
#define BITMASK_REGISTER_CPL  0x00000000FF0000
#define BITMASK_REGISTER_ARCH 0x0000000000FFFF
enum class __register__ : uint64_t {
    invalid = 0,
    /* x86 */           rax   = 0x00010008030003,
    /* x86 */           rbx   = 0x00020008030003,
    /* x86 */           rcx   = 0x00030008030003,
    /* x86 */           rdx   = 0x00040008030003,
    /* x86 */           rsi   = 0x00050008030003,
    /* x86 */           rdi   = 0x00060008030003,
    /* x86 */           rbp   = 0x00070008030003,
    /* x86 */           rsp   = 0x00080008030003,
    /* ARM */           r0    = 0x0001000003000C,
    /* ARM */           r1    = 0x0002000003000C,
    /* ARM */           r2    = 0x0003000003000C,
    /* ARM */           r3    = 0x0004000003000C,
    /* ARM */           r4    = 0x0005000003000C,
    /* ARM */           r5    = 0x0006000003000C,
    /* ARM */           r6    = 0x0007000003000C,
    /* ARM */           r7    = 0x0008000003000C,
    /* x86 and ARM */   r8    = 0x0009000803000D,
    /* x86 and ARM */   r9    = 0x000a000803000D,
    /* x86 and ARM */   r10   = 0x000b000803000D,
    /* x86 and ARM */   r11   = 0x000c000803000D,
    /* x86 and ARM */   r12   = 0x000d000803000D,
    /* x86 and ARM */   r13   = 0x000e000803000D,
    /* x86 and ARM */   r14   = 0x000f000803000D,
    /* x86 and ARM */   r15   = 0x0010000803000D,
    /* x86 */           rip   = 0x00110408030003,
    /* x86 */           cr0   = 0x00120208000003,
    /* x86 */           cr1   = 0x00130208000001,
    /* x86 */           cr2   = 0x00140208000001,
    /* x86 */           cr3   = 0x00150208000001,
    /* x86 */           cr4   = 0x00160208000001,
    /* x86 */           cr5   = 0x00170208000001,
    /* x86 */           cr6   = 0x00180208000001,
    /* x86 */           cr7   = 0x00190208000001,
    /* x86 */           cr8   = 0x001a0208000001,
    /* x86 */           cr9   = 0x001b0208000001,
    /* x86 */           cr10  = 0x001c0208000001,
    /* x86 */           cr11  = 0x001d0208000001,
    /* x86 */           cr12  = 0x001e0208000001,
    /* x86 */           cr13  = 0x001f0208000001,
    /* x86 */           cr14  = 0x00200208000001,
    /* x86 */           cr15  = 0x00210208000001,
    /* x86 */           EFER  = 0x00220108000001,
    /* x86 */           DR0   = 0x00230608030001,
    /* x86 */           DR1   = 0x00240608030001,
    /* x86 */           DR2   = 0x00250608030001,
    /* x86 */           DR3   = 0x00260608030001,
    /* x86 */           DR6   = 0x00270608030001,
    /* x86 */           DR7   = 0x00280608030001,
    /* x86 */           GDTR  = 0x0029040A000003,
    /* x86 */           IDTR  = 0x002a040A000003,
    /* x86 */           LDTR  = 0x002b040A000003,
    /* x86 */           TR    = 0x002c0302000003,
    /* x86 */           CS    = 0x002d0302000003,
    /* x86 */           DS    = 0x002e0302000003,
    /* x86 */           SS    = 0x002f0302000003,
    /* x86 */           ES    = 0x00300302000003,
    /* x86 */           FS    = 0x00310302000003,
    /* x86 */           GS    = 0x00320302000003,
    /* x86 */           xmm0  = 0x00400508030001,
    /* x86 */           xmm1  = 0x00410508030001,
    /* x86 */           xmm2  = 0x00420508030001,
    /* x86 */           xmm3  = 0x00430508030001,
    /* x86 */           xmm4  = 0x00440508030001,
    /* x86 */           xmm5  = 0x00450508030001,
    /* x86 */           xmm6  = 0x00460508030001,
    /* x86 */           xmm7  = 0x00470508030001,
    /* x86 */           xmm8  = 0x00480508030001,
    /* x86 */           xmm9  = 0x00490508030001,
    /* x86 */           xmm10 = 0x004a0508030001,
    /* x86 */           xmm11 = 0x004b0508030001,
    /* x86 */           xmm12 = 0x004c0508030001,
    /* x86 */           xmm13 = 0x004d0508030001,
    /* x86 */           xmm14 = 0x004e0508030001,
    /* x86 */           xmm15 = 0x004f0508030001,
};
extern std::vector<__register__> registers;
extern __register__ StackPointer;

uint64_t getx86MSR(__register__ reg);

enum class storageType : uint8_t {
    INVALID,REGISTER,MEMORY,MEMORY_ABSOLUTE,IMMEDIATE,SYMBOL
};

__register__ registerID(std::string name);
std::string registerNAME(__register__ reg);

class variable
{
public:
    type* dataType = nullptr;
    std::string name;
    std::string symbol;
    bool doExport = false;
    int8_t access = 0;
    bool isStatic = false;
    bool usedAutoStorage = false;
    uint64_t immediateValue;
    std::vector<variable*> children;//stores members of this variable (ex. gdt.size or var->name)
    variable* parent;//only present for child variables
    //storage
    storageType storage = storageType::INVALID;
    __register__ reg;
    int64_t offset;
    /*
        if the variable is inside a function this offset 
        is relative to rsp meaning that rsp-offset is the start of the variables data

        if the variable is a member to a class this offset 
        is relative to the start of the class

        if the variable is not inside a function offset is the absolute memory address at which the variable resides
        in which case a negative value is illegal
        in this case offset is to be treated as if it was of type uint64_t rather than int64_t

        if storage is MEMORY_ABSOLUTE then offset is always the absolute memory address at which the variable resides

        if storage!=memory && storage!=memory_absolute this value is to be ignored
    */
};