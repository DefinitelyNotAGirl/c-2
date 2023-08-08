/*
 * Created Date: Thursday August 3rd 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 3rd 2023 1:59:47 pm
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

#include "pops.h"

void mov(__register__ src,__register__ dst)
{
    if(src != dst)
        code->push_back(getIndent()+"mov %"+registerNAME(src)+", %"+registerNAME(dst));
}

void mov(location src, location dst)
{
    code->push_back(getIndent()+"mov "+src.expr()+", "+dst.expr());
}

void mov(uint64_t src, __register__ dst)
{
    if(src != 0)
        code->push_back(getIndent()+"mov $"+std::to_string(src)+", %"+registerNAME(dst));
    else
        code->push_back(getIndent()+"xor %"+registerNAME(dst)+", %"+registerNAME(dst));
}

void mov(location src, __register__ dst)
{
    code->push_back(getIndent()+"mov "+src.expr()+", %"+registerNAME(dst));
}

void mov(__register__ src, location dst)
{
    code->push_back(getIndent()+"mov %"+registerNAME(src)+", "+dst.expr());
}

void mov(variable* src, __register__ dst)
{
    if(src->storage == storageType::REGISTER)
    {
        mov(src->reg,dst);
    }
    else if(src->storage == storageType::MEMORY)
    {
        mov(location(__register__::rsp,src->offset),dst);
    }
    else if(src->storage == storageType::MEMORY_ABSOLUTE)
    {
        mov(location(src->offset),dst);
    }
    else if(src->storage == storageType::IMMEDIATE)
    {
        mov(src->immediateValue,dst);
    }
}
void mov(__register__ src, variable* dst)
{
    if(dst->storage == storageType::REGISTER)
    {
        mov(src,dst->reg);
    }
    else if(dst->storage == storageType::MEMORY)
    {
        mov(src,location(__register__::rsp,dst->offset));
    }
    else if(dst->storage == storageType::MEMORY_ABSOLUTE)
    {
        mov(src,location(dst->offset));
    }
    else if(dst->storage == storageType::IMMEDIATE)
    {
        std::cout << "blub storage type immediate (1)" << std::endl;
    }
}

void mov(variable* src, variable* dst)
{
    if(dst->storage == storageType::REGISTER && src->storage == storageType::REGISTER)
        mov(src->reg,dst->reg);
    else if(dst->storage == storageType::REGISTER && src->storage == storageType::MEMORY)
        mov(location(__register__::rsp,src->offset),dst->reg);
    else if(src->storage == storageType::REGISTER && dst->storage == storageType::MEMORY)
        mov(src->reg,location(__register__::rsp,dst->offset));
    else if(dst->storage == storageType::MEMORY && src->storage == storageType::MEMORY)
        mov(location(__register__::rsp,src->offset),location(__register__::rsp,dst->offset));
    else if(dst->storage == storageType::MEMORY_ABSOLUTE && src->storage == storageType::REGISTER)
        mov(src->reg,location(dst->offset));
    else if(src->storage == storageType::MEMORY_ABSOLUTE && dst->storage == storageType::REGISTER)
        mov(location(src->offset),dst->reg);
    else if(src->storage == storageType::MEMORY_ABSOLUTE && dst->storage == storageType::MEMORY_ABSOLUTE)
    {
        //cant move directly
        //need intermediate register
        __register__ reg = fstore->getFreeRegister();
        bool rInvalid = false;
        if(reg == __register__::invalid)
        {
            rInvalid = true;
            reg = __register__::rax;
            saveRegister(reg);
        }
        mov(src,reg);
        mov(reg,dst);
        if(rInvalid);
            restoreRegisters();
    }
    else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::MEMORY_ABSOLUTE)
    {
        PRINT_DEBUG
        //cant move directly
        //need intermediate register
        __register__ reg = fstore->getFreeRegister();
        bool rInvalid = false;
        if(reg == __register__::invalid)
        {
            rInvalid = true;
            reg = __register__::rax;
            saveRegister(reg);
        }
        mov(src,reg);
        mov(reg,dst);
        if(rInvalid);
            restoreRegisters();
    }
    else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::MEMORY)
    {
        PRINT_DEBUG
        //cant move directly
        //need intermediate register
        __register__ reg = fstore->getFreeRegister();
        bool rInvalid = false;
        if(reg == __register__::invalid)
        {
            rInvalid = true;
            reg = __register__::rax;
            saveRegister(reg);
        }
        mov(src,reg);
        mov(reg,dst);
        if(rInvalid);
            restoreRegisters();
    }
    else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::REGISTER)
    {
        mov(src->immediateValue,dst->reg);
    }
    else
    {
        std::cout << "well, ig im dumb LMFAO" << std::endl;
        std::cout << "src: " << (uint64_t)src->storage << std::endl;
        std::cout << "dst: " << (uint64_t)dst->storage << std::endl;
    }
}

void mov(uint64_t src,location dst)
{
    //cant move directly
    //need intermediate register
    __register__ reg = fstore->getFreeRegister();
    bool rInvalid = false;
    if(reg == __register__::invalid)
    {
        rInvalid = true;
        reg = __register__::rax;
        saveRegister(reg);
    }
    mov(src,reg);
    mov(reg,dst);
    if(rInvalid);
        restoreRegisters();
}

void mov(variable* src,location dst)
{
    if(src->storage == storageType::REGISTER)
        mov(src->reg,dst);
    else
    {
        //cant move directly
        //need intermediate register
        __register__ reg = fstore->getFreeRegister();
        bool rInvalid = false;
        if(reg == __register__::invalid)
        {
            rInvalid = true;
            reg = __register__::rax;
            saveRegister(reg);
        }
        mov(src,reg);
        mov(reg,dst);
        if(rInvalid);
            restoreRegisters();
    }
}

void mov(location src,variable* dst)
{
    if(dst->storage == storageType::REGISTER)
        mov(src,dst->reg);
    else
    {
        //cant move directly
        //need intermediate register
        __register__ reg = fstore->getFreeRegister();
        bool rInvalid = false;
        if(reg == __register__::invalid)
        {
            rInvalid = true;
            reg = __register__::rax;
            saveRegister(reg);
        }
        mov(src,reg);
        mov(reg,dst);
        if(rInvalid);
            restoreRegisters();
    }
}