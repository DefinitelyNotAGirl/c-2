/*
 * Created Date: Wednesday August 2nd 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday August 2nd 2023 1:27:22 am
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

#include <storage.h>

functionStorage::~functionStorage()
{
    free(this->regData);
}

functionStorage::functionStorage()
{
    uint64_t registers = 58;// 58 registers available for x86
    this->regData = (uint64_t*)calloc(1,registers*16);
    uint64_t i = 0;
    this->regData[i+=2] = (uint64_t)__register__::rax;
    this->regData[i+=2] = (uint64_t)__register__::rbx;
    this->regData[i+=2] = (uint64_t)__register__::rcx;
    this->regData[i+=2] = (uint64_t)__register__::rdx;
    this->regData[i+=2] = (uint64_t)__register__::rsi;
    this->regData[i+=2] = (uint64_t)__register__::rdi;
    this->regData[i+=2] = (uint64_t)__register__::rbp;
    this->regData[i+=2] = (uint64_t)__register__::rsp;
    this->regData[i+=2] = (uint64_t)__register__::r0;
    this->regData[i+=2] = (uint64_t)__register__::r1;
    this->regData[i+=2] = (uint64_t)__register__::r2;
    this->regData[i+=2] = (uint64_t)__register__::r3;
    this->regData[i+=2] = (uint64_t)__register__::r4;
    this->regData[i+=2] = (uint64_t)__register__::r5;
    this->regData[i+=2] = (uint64_t)__register__::r6;
    this->regData[i+=2] = (uint64_t)__register__::r7;
    this->regData[i+=2] = (uint64_t)__register__::r8;
    this->regData[i+=2] = (uint64_t)__register__::r9;
    this->regData[i+=2] = (uint64_t)__register__::r10;
    this->regData[i+=2] = (uint64_t)__register__::r11;
    this->regData[i+=2] = (uint64_t)__register__::r12;
    this->regData[i+=2] = (uint64_t)__register__::r13;
    this->regData[i+=2] = (uint64_t)__register__::r14;
    this->regData[i+=2] = (uint64_t)__register__::r15;
    this->regData[i+=2] = (uint64_t)__register__::rip;
    this->regData[i+=2] = (uint64_t)__register__::cr0;
    this->regData[i+=2] = (uint64_t)__register__::cr1;
    this->regData[i+=2] = (uint64_t)__register__::cr2;
    this->regData[i+=2] = (uint64_t)__register__::cr3;
    this->regData[i+=2] = (uint64_t)__register__::cr4;
    this->regData[i+=2] = (uint64_t)__register__::cr5;
    this->regData[i+=2] = (uint64_t)__register__::cr6;
    this->regData[i+=2] = (uint64_t)__register__::cr7;
    this->regData[i+=2] = (uint64_t)__register__::cr8;
    this->regData[i+=2] = (uint64_t)__register__::cr9;
    this->regData[i+=2] = (uint64_t)__register__::cr10;
    this->regData[i+=2] = (uint64_t)__register__::cr11;
    this->regData[i+=2] = (uint64_t)__register__::cr12;
    this->regData[i+=2] = (uint64_t)__register__::cr13;
    this->regData[i+=2] = (uint64_t)__register__::cr14;
    this->regData[i+=2] = (uint64_t)__register__::cr15;
    this->regData[i+=2] = (uint64_t)__register__::EFER;
    this->regData[i+=2] = (uint64_t)__register__::DR0;
    this->regData[i+=2] = (uint64_t)__register__::DR1;
    this->regData[i+=2] = (uint64_t)__register__::DR2;
    this->regData[i+=2] = (uint64_t)__register__::DR3;
    this->regData[i+=2] = (uint64_t)__register__::DR6;
    this->regData[i+=2] = (uint64_t)__register__::DR7;
    this->regData[i+=2] = (uint64_t)__register__::GDTR;
    this->regData[i+=2] = (uint64_t)__register__::IDTR;
    this->regData[i+=2] = (uint64_t)__register__::LDTR;
    this->regData[i+=2] = (uint64_t)__register__::TR;
    this->regData[i+=2] = (uint64_t)__register__::CS;
    this->regData[i+=2] = (uint64_t)__register__::DS;
    this->regData[i+=2] = (uint64_t)__register__::SS;
    this->regData[i+=2] = (uint64_t)__register__::ES;
    this->regData[i+=2] = (uint64_t)__register__::FS;
    this->regData[i+=2] = (uint64_t)__register__::GS;
}

void functionStorage::registerStatus(__register__ reg,uint64_t value)
{
    switch(reg)
    {
        case(__register__::rax):
            this->regData[1] = value;
            break;
        case(__register__::rbx):
            this->regData[3] = value;
            break;
        case(__register__::rcx):
            this->regData[5] = value;
            break;
        case(__register__::rdx):
            this->regData[7] = value;
            break;
        case(__register__::rsi):
            this->regData[9] = value;
            break;
        case(__register__::rdi):
            this->regData[11] = value;
            break;
        case(__register__::rbp):
            this->regData[13] = value;
            break;
        case(__register__::rsp):
            this->regData[15] = value;
            break;
        case(__register__::r0):
            this->regData[17] = value;
            break;
        case(__register__::r1):
            this->regData[19] = value;
            break;
        case(__register__::r2):
            this->regData[21] = value;
            break;
        case(__register__::r3):
            this->regData[23] = value;
            break;
        case(__register__::r4):
            this->regData[25] = value;
            break;
        case(__register__::r5):
            this->regData[27] = value;
            break;
        case(__register__::r6):
            this->regData[29] = value;
            break;
        case(__register__::r7):
            this->regData[31] = value;
            break;
        case(__register__::r8):
            this->regData[33] = value;
            break;
        case(__register__::r9):
            this->regData[35] = value;
            break;
        case(__register__::r10):
            this->regData[37] = value;
            break;
        case(__register__::r11):
            this->regData[39] = value;
            break;
        case(__register__::r12):
            this->regData[41] = value;
            break;
        case(__register__::r13):
            this->regData[43] = value;
            break;
        case(__register__::r14):
            this->regData[45] = value;
            break;
        case(__register__::r15):
            this->regData[47] = value;
            break;
        case(__register__::rip):
            this->regData[49] = value;
            break;
        case(__register__::cr0):
            this->regData[51] = value;
            break;
        case(__register__::cr1):
            this->regData[53] = value;
            break;
        case(__register__::cr2):
            this->regData[55] = value;
            break;
        case(__register__::cr3):
            this->regData[57] = value;
            break;
        case(__register__::cr4):
            this->regData[59] = value;
            break;
        case(__register__::cr5):
            this->regData[61] = value;
            break;
        case(__register__::cr6):
            this->regData[63] = value;
            break;
        case(__register__::cr7):
            this->regData[65] = value;
            break;
        case(__register__::cr8):
            this->regData[67] = value;
            break;
        case(__register__::cr9):
            this->regData[69] = value;
            break;
        case(__register__::cr10):
            this->regData[71] = value;
            break;
        case(__register__::cr11):
            this->regData[73] = value;
            break;
        case(__register__::cr12):
            this->regData[75] = value;
            break;
        case(__register__::cr13):
            this->regData[77] = value;
            break;
        case(__register__::cr14):
            this->regData[79] = value;
            break;
        case(__register__::cr15):
            this->regData[81] = value;
            break;
        case(__register__::EFER):
            this->regData[83] = value;
            break;
        case(__register__::DR0):
            this->regData[85] = value;
            break;
        case(__register__::DR1):
            this->regData[87] = value;
            break;
        case(__register__::DR2):
            this->regData[89] = value;
            break;
        case(__register__::DR3):
            this->regData[91] = value;
            break;
        case(__register__::DR6):
            this->regData[93] = value;
            break;
        case(__register__::DR7):
            this->regData[95] = value;
            break;
        case(__register__::GDTR):
            this->regData[97] = value;
            break;
        case(__register__::IDTR):
            this->regData[99] = value;
            break;
        case(__register__::LDTR):
            this->regData[101] = value;
            break;
        case(__register__::TR):
            this->regData[103] = value;
            break;
        case(__register__::CS):
            this->regData[105] = value;
            break;
        case(__register__::DS):
            this->regData[107] = value;
            break;
        case(__register__::SS):
            this->regData[109] = value;
            break;
        case(__register__::ES):
            this->regData[111] = value;
            break;
        case(__register__::FS):
            this->regData[113] = value;
            break;
        case(__register__::GS):
            this->regData[115] = value;
            break;
        default:
            std::cout << "Invalid register" << std::endl;
    }
}

uint64_t functionStorage::registerStatus(__register__ reg)
{
    switch(reg)
    {
        case(__register__::rax):
            return this->regData[1];
        case(__register__::rbx):
            return this->regData[3];
        case(__register__::rcx):
            return this->regData[5];
        case(__register__::rdx):
            return this->regData[7];
        case(__register__::rsi):
            return this->regData[9];
        case(__register__::rdi):
            return this->regData[11];
        case(__register__::rbp):
            return this->regData[13];
        case(__register__::rsp):
            return this->regData[15];
        case(__register__::r0):
            return this->regData[17];
        case(__register__::r1):
            return this->regData[19];
        case(__register__::r2):
            return this->regData[21];
        case(__register__::r3):
            return this->regData[23];
        case(__register__::r4):
            return this->regData[25];
        case(__register__::r5):
            return this->regData[27];
        case(__register__::r6):
            return this->regData[29];
        case(__register__::r7):
            return this->regData[31];
        case(__register__::r8):
            return this->regData[33];
        case(__register__::r9):
            return this->regData[35];
        case(__register__::r10):
            return this->regData[37];
        case(__register__::r11):
            return this->regData[39];
        case(__register__::r12):
            return this->regData[41];
        case(__register__::r13):
            return this->regData[43];
        case(__register__::r14):
            return this->regData[45];
        case(__register__::r15):
            return this->regData[47];
        case(__register__::rip):
            return this->regData[49];
        case(__register__::cr0):
            return this->regData[51];
        case(__register__::cr1):
            return this->regData[53];
        case(__register__::cr2):
            return this->regData[55];
        case(__register__::cr3):
            return this->regData[57];
        case(__register__::cr4):
            return this->regData[59];
        case(__register__::cr5):
            return this->regData[61];
        case(__register__::cr6):
            return this->regData[63];
        case(__register__::cr7):
            return this->regData[65];
        case(__register__::cr8):
            return this->regData[67];
        case(__register__::cr9):
            return this->regData[69];
        case(__register__::cr10):
            return this->regData[71];
        case(__register__::cr11):
            return this->regData[73];
        case(__register__::cr12):
            return this->regData[75];
        case(__register__::cr13):
            return this->regData[77];
        case(__register__::cr14):
            return this->regData[79];
        case(__register__::cr15):
            return this->regData[81];
        case(__register__::EFER):
            return this->regData[83];
        case(__register__::DR0):
            return this->regData[85];
        case(__register__::DR1):
            return this->regData[87];
        case(__register__::DR2):
            return this->regData[89];
        case(__register__::DR3):
            return this->regData[91];
        case(__register__::DR6):
            return this->regData[93];
        case(__register__::DR7):
            return this->regData[95];
        case(__register__::GDTR):
            return this->regData[97];
        case(__register__::IDTR):
            return this->regData[99];
        case(__register__::LDTR):
            return this->regData[101];
        case(__register__::TR):
            return this->regData[103];
        case(__register__::CS):
            return this->regData[105];
        case(__register__::DS):
            return this->regData[107];
        case(__register__::SS):
            return this->regData[109];
        case(__register__::ES):
            return this->regData[111];
        case(__register__::FS):
            return this->regData[113];
        case(__register__::GS):
            return this->regData[115];
        default:
            return 2;
    }
    return 2;
}

void functionStorage::setStorage(variable* var)
{
    using enum __register__;
    if(this->ABI == 1)
    {
        //SystemV amd64 ABI
        if(var->dataType->members.size() == 0 && var->dataType->size <= 8)
        {
            //64 bit primitive

            //scratch registers
            if(this->registerStatus(rax)==0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rax;
                this->registerStatus(rax,1);
                return;
            }
            else if(this->registerStatus(rdi) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rdi;
                this->registerStatus(rdi,1);
                return;
            }
            else if(this->registerStatus(rsi) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rsi;
                this->registerStatus(rsi,1);
                return;
            }
            else if(this->registerStatus(rdx) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rdx;
                this->registerStatus(rdx,1);
                return;
            }
            else if(this->registerStatus(rcx) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rcx;
                this->registerStatus(rcx,1);
                return;
            }
            else if(this->registerStatus(r8) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r8;
                this->registerStatus(r8,1);
                return;
            }
            else if(this->registerStatus(r9) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r9;
                this->registerStatus(r9,1);
                return;
            }
            else if(this->registerStatus(r10) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r10;
                this->registerStatus(r10,1);
                return;
            }
            else if(this->registerStatus(r11) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r11;
                this->registerStatus(r11,1);
                return;
            }
            //preserved registers
            else if(this->registerStatus(rbx) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rbx;
                this->registerStatus(rbx,1);
                return;
            }
            else if(this->registerStatus(rsp) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rsp;
                this->registerStatus(rsp,1);
                return;
            }
            else if(this->registerStatus(rbp) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rbp;
                this->registerStatus(rbp,1);
                return;
            }
            else if(this->registerStatus(r12) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r12;
                this->registerStatus(r12,1);
                return;
            }
            else if(this->registerStatus(r13) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r13;
                this->registerStatus(r13,1);
                return;
            }
            else if(this->registerStatus(r14) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r14;
                this->registerStatus(r14,1);
                return;
            }
            else if(this->registerStatus(r15) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r15;
                this->registerStatus(r15,1);
                return;
            }
        }

        //no registers aviable, store in stack
        var->storage = storageType::MEMORY;
        //align stack size
        this->stackSize += this->stackSize%var->dataType->size;
        var->offset=this->stackSize;
        this->stackSize+=var->dataType->size;
    }
    else if(this->ABI == 2)
    {
        //Microsoft x64 ABI
        if(var->dataType->members.size() == 0 && var->dataType->size <= 8)
        {
            if(this->registerStatus(rax) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rax;
                this->registerStatus(rax,1);
                return;
            }
            else if(this->registerStatus(rcx) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rcx;
                this->registerStatus(rcx,1);
                return;
            }
            else if(this->registerStatus(rdx) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rdx;
                this->registerStatus(rdx,1);
                return;
            }
            else if(this->registerStatus(r8) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r8;
                this->registerStatus(r8,1);
                return;
            }
            else if(this->registerStatus(r9) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r9;
                this->registerStatus(r9,1);
                return;
            }
            else if(this->registerStatus(r10) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r10;
                this->registerStatus(r10,1);
                return;
            }
            else if(this->registerStatus(r11) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r11;
                this->registerStatus(r11,1);
                return;
            }	
            else if(this->registerStatus(rbx) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rbx;
                this->registerStatus(rbx,1);
                return;
            }
            else if(this->registerStatus(rdi) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rdi;
                this->registerStatus(rdi,1);
                return;
            }
            else if(this->registerStatus(rsi) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rsi;
                this->registerStatus(rsi,1);
                return;
            }
            else if(this->registerStatus(rsp) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rsp;
                this->registerStatus(rsp,1);
                return;
            }
            else if(this->registerStatus(rbp) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = rbp;
                this->registerStatus(rbp,1);
                return;
            }
            else if(this->registerStatus(r12) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r12;
                this->registerStatus(r12,1);
                return;
            }
            else if(this->registerStatus(r13) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r13;
                this->registerStatus(r13,1);
                return;
            }
            else if(this->registerStatus(r14) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r14;
                this->registerStatus(r14,1);
                return;
            }
            else if(this->registerStatus(r15) == 0)
            {
                var->storage = storageType::REGISTER;
                var->reg = r15;
                this->registerStatus(r15,1);
                return;
            }	
        }

        //no registers aviable, store in stack
        var->storage = storageType::MEMORY;
        //align stack size
        this->stackSize += this->stackSize%var->dataType->size;
        var->offset=this->stackSize;
        this->stackSize+=var->dataType->size;
    }
    else
    {
        std::cout << "ERROR: unsupported ABI: " << this->ABI << std::endl;
    }
}
