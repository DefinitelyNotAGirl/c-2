/**
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 25th 2023 10:56:50 pm
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
    0x********00: invalid
    0x********01: multiple architectures
    0x********02: x86
    0x********03: ARM
privilege levels:
    0x******00**: cpl 0
    0x******01**: cpl 1
    0x******02**: cpl 2
    0x******03**: cpl 3
register size:
    0x****XX****: XX is the size of the register in bytes
*/
enum class __register__ : uint64_t {
    invalid = 0,
    /* x86 */           rax  = 0x0001080302,
    /* x86 */           rbx  = 0x0002080302,
    /* x86 */           rcx  = 0x0003080302,
    /* x86 */           rdx  = 0x0004080302,
    /* x86 */           rsi  = 0x0005080302,
    /* x86 */           rdi  = 0x0006080302,
    /* x86 */           rbp  = 0x0007080302,
    /* x86 */           rsp  = 0x0008080302,
    /* ARM */           r0   = 0x0001000303,
    /* ARM */           r1   = 0x0002000303,
    /* ARM */           r2   = 0x0003000303,
    /* ARM */           r3   = 0x0004000303,
    /* ARM */           r4   = 0x0005000303,
    /* ARM */           r5   = 0x0006000303,
    /* ARM */           r6   = 0x0007000303,
    /* ARM */           r7   = 0x0008000303,
    /* x86 and ARM */   r8   = 0x0009080301,
    /* x86 and ARM */   r9   = 0x000a080301,
    /* x86 and ARM */   r10  = 0x000b080301,
    /* x86 and ARM */   r11  = 0x000c080301,
    /* x86 and ARM */   r12  = 0x000d080301,
    /* x86 and ARM */   r13  = 0x000e080301,
    /* x86 and ARM */   r14  = 0x000f080301,
    /* x86 and ARM */   r15  = 0x0010080301,
    /* x86 */           rip  = 0x0011080302,
    /* x86 */           cr0  = 0x0012080002,
    /* x86 */           cr1  = 0x0013080002,
    /* x86 */           cr2  = 0x0014080002,
    /* x86 */           cr3  = 0x0015080002,
    /* x86 */           cr4  = 0x0016080002,
    /* x86 */           cr5  = 0x0017080002,
    /* x86 */           cr6  = 0x0018080002,
    /* x86 */           cr7  = 0x0019080002,
    /* x86 */           cr8  = 0x001a080002,
    /* x86 */           cr9  = 0x001b080002,
    /* x86 */           cr10 = 0x001c080002,
    /* x86 */           cr11 = 0x001d080002,
    /* x86 */           cr12 = 0x001e080002,
    /* x86 */           cr13 = 0x001f080002,
    /* x86 */           cr14 = 0x0020080002,
    /* x86 */           cr15 = 0x0021080002,
    /* x86 */           EFER = 0x0022080002,
    /* x86 */           DR0  = 0x0023080302,
    /* x86 */           DR1  = 0x0024080302,
    /* x86 */           DR2  = 0x0025080302,
    /* x86 */           DR3  = 0x0026080302,
    /* x86 */           DR6  = 0x0027080302,
    /* x86 */           DR7  = 0x0028080302,
    /* x86 */           GDTR = 0x00290A0002,
    /* x86 */           IDTR = 0x002a0A0002,
    /* x86 */           LDTR = 0x002b0A0002,
    /* x86 */           TR   = 0x002c020002,
    /* x86 */           CS   = 0x002d020002,
    /* x86 */           DS   = 0x002e020002,
    /* x86 */           SS   = 0x002f020002,
    /* x86 */           ES   = 0x0030020002,
    /* x86 */           FS   = 0x0031020002,
    /* x86 */           GS   = 0x0032020002
};

enum class storageType : uint8_t {
    INVALID,REGISTER,MEMORY,MEMORY_ABSOLUTE,IMMEDIATE
};

__register__ registerID(std::string name);
std::string registerNAME(__register__ reg);

class variable
{
public:
    type* dataType = nullptr;
    std::string name;
    uint64_t immediateValue;
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