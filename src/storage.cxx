/*
 * Created Date: Wednesday August 2nd 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday September 17th 2023 5:19:35 am
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
#include <codegen.h>

functionStorage::~functionStorage()
{
    free(this->regData);
}

functionStorage::functionStorage()
{
    uint64_t i = 1;
    for(__register__ I : registers)
        if((uint64_t)I & ARCH_REGISTER_MASK != 0)
            i++;
    this->regData = (uint64_t*)malloc(i*24);
    memset(this->regData,0,i*24);
    uint64_t rd = (uint64_t)this->regData;
    for(__register__ I : registers)
    {
        if((uint64_t)I & ARCH_REGISTER_MASK != 0)
        {
            *((uint64_t*)rd) = (uint64_t)I;
            rd+=(uint64_t)24;
        }
    }
}

//void functionStorage::registerStoredVariable(__register__ reg,variable* var)
//{
//    if(options::ddebug)
//        std::cout << "fetching variable stored in " << registerNAME(reg) << std::endl;
//    uint64_t rd = (uint64_t)this->regData;
//    while(*((uint64_t*)(rd)) != 0)
//    {
//        if(*((uint64_t*)(rd)) == (uint64_t)reg)
//        {
//            *((variable*)(rd+16)) = var;
//            return;
//        }
//        else
//            rd+=24;
//    }
//    std::cout << "ERROR, register not found!" << std::endl;
//}
//
//variable* functionStorage::registerStoredVariable(__register__ reg)
//{
//    if(options::ddebug)
//        std::cout << "fetching variable stored in " << registerNAME(reg) << std::endl;
//    uint64_t rd = (uint64_t)this->regData;
//    while(*((uint64_t*)(rd)) != 0)
//    {
//        if(*((uint64_t*)(rd)) == (uint64_t)reg)
//        {
//            return *((variable*)(rd+16));
//        }
//        else
//            rd+=24;
//    }
//    std::cout << "ERROR, register not found!" << std::endl;
//}

void functionStorage::registerStatus(__register__ reg,uint64_t value)
{
    if(options::ddebug)
        std::cout << "setting status of " << registerNAME(reg) << " to " << value << std::endl;
    uint64_t rd = (uint64_t)this->regData;
    while(*((uint64_t*)(rd)) != 0)
    {
        if(*((uint64_t*)(rd)) == (uint64_t)reg)
        {
            *((uint64_t*)(rd+8)) = value;
            return;
        }
        else
            rd+=24;
    }
    std::cout << "ERROR, register not found!" << std::endl;
}

uint64_t functionStorage::registerStatus(__register__ reg)
{
    uint64_t rd = (uint64_t)this->regData;
    while(*((uint64_t*)(rd)) != 0)
    {
        if(*((uint64_t*)(rd)) == (uint64_t)reg)
        {
            return *((uint64_t*)(rd+8));
        }
        else
            rd+=24;
    }
    return 2;
}

void functionStorage::setStorage(function* func, variable* var)
{
    using enum __register__;
    if(var->dataType->regMode == 1)
    {
        for(__register__ I : func->abi->VolatileRegisters)
        {
            if(!(
                   I == func->abi->integerReturn
                || I == rdx
                || I == rcx
                || I == rip
                || I == rbp
            )){if((uint64_t)((uint64_t)I & BITMASK_REGISTER_TYPE) == 0x00)
            {
                if(this->registerStatus(I) == 0)
                {
                    this->registerStatus(I,1);
                    var->storage = storageType::REGISTER;
                    var->reg = I;
                    return;
                }
            }}
        }
        for(__register__ I : func->abi->nonVolatile)
        {
            if(!(
                   I == func->abi->integerReturn
                || I == rdx
                || I == rcx
                || I == rip
                || I == rbp
            )){if((uint64_t)((uint64_t)I & BITMASK_REGISTER_TYPE) == 0x00)
            {
                if(this->registerStatus(I) == 0)
                {
                    this->registerStatus(I,1);
                    var->storage = storageType::REGISTER;
                    var->reg = I;
                    return;
                }
            }}
        }
    }
    else if(var->dataType->regMode == 2)
    {
        for(__register__ I : func->abi->VolatileRegisters)
        {
            if(!(
                   I == func->abi->floatReturn
            )){if((uint64_t)((uint64_t)I & BITMASK_REGISTER_TYPE) == 0x500000000)
            {
                if(this->registerStatus(I) == 0)
                {
                    this->registerStatus(I,1);
                    var->storage = storageType::REGISTER;
                    var->reg = I;
                    return;
                }
            }}
        }
        for(__register__ I : func->abi->nonVolatile)
        {
            if(!(
                   I == func->abi->floatReturn
            )){if((uint64_t)((uint64_t)I & BITMASK_REGISTER_TYPE) == 0x500000000)
            {
                if(this->registerStatus(I) == 0)
                {
                    this->registerStatus(I,1);
                    var->storage = storageType::REGISTER;
                    var->reg = I;
                    return;
                }
            }}
        }
    }

    //no registers aviable or too large, store in stack
    var->storage = storageType::MEMORY;
    //align stack size
    this->stackSize += this->stackSize%var->dataType->size;
    var->offset=this->stackSize;
    var->reg = StackPointer;
    this->stackSize+=var->dataType->size;
}

__register__ functionStorage::getFreeRegister()
{
    using enum __register__;
    if(this->registerStatus(rcx) == 0)
        return rcx;
    else if(this->registerStatus(rdx) == 0)
        return rdx;
    else if(this->registerStatus(r8) == 0)
        return r8;
    else if(this->registerStatus(r9) == 0)
        return r9;
    else if(this->registerStatus(r10) == 0)
        return r10;
    else if(this->registerStatus(r11) == 0)
        return r11;	
    else if(this->registerStatus(rbx) == 0)
        return rbx;
    else if(this->registerStatus(rdi) == 0)
        return rdi;
    else if(this->registerStatus(rsi) == 0)
        return rsi;
    else if(this->registerStatus(rsp) == 0)
        return rsp;
    else if(this->registerStatus(rbp) == 0)
        return rbp;
    else if(this->registerStatus(r12) == 0)
        return r12;
    else if(this->registerStatus(r13) == 0)
        return r13;
    else if(this->registerStatus(r14) == 0)
        return r14;
    else if(this->registerStatus(r15) == 0)
        return r15;
    else if(this->registerStatus(rax) == 0)
        return rax;
    else
        return invalid;
}

void functionStorage::setArgumentStorage(variable* var)
{
    using enum __register__;
    if(this->ABI == 1)
    {
        //SystemV amd64 ABI
        if(var->dataType->members.size() == 0 && var->dataType->size <= 8)
        {
            //64 bit primitive

            //scratch registers
            if(this->registerStatus(rdi) == 0)
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
