/*
 * Created Date: Thursday August 3rd 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday December 25th 2023 12:32:29 am
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

#include <compiler.h>
#include <codegen.h>
#include <error.h>
#include <colors.h>

namespace x86_64
{
    variable* _0 = getImmediateVariable(0);
    void mov(uint64_t,__register__);
    
    void writeMSR(__register__ src)
    {
        std::cout << "wrmsr" << std::endl;
        using enum __register__;
        pushRegSave();
        if(fstore->registerStatus(rcx) != 0)
            saveRegister(rcx);
        if(fstore->registerStatus(rdx) != 0)
            saveRegister(rdx);
        x86_64::mov((uint64_t)0,rdx);
        setANB(16);
        x86_64::mov(getx86MSR(src),rcx);
        popANB();
        code->push_back(getIndent()+"wrmsr");
        restoreRegisters();
    }

    void readMSR(__register__ dst)
    {
        std::cout << "rdmsr" << std::endl;
        using enum __register__;
        pushRegSave();
        if(fstore->registerStatus(rcx) != 0)
            saveRegister(rcx);
        setANB(16);
        x86_64::mov(getx86MSR(dst),rcx);
        popANB();
        code->push_back(getIndent()+"rdmsr");
        restoreRegisters();
    }

    void mov(__register__ src,__register__ dst)
    {
        using enum __register__;
        if(
            ((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)src) == 0x00) 
            && 
            (((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)dst) == 0x00) || ((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)dst) == 0x02) || ((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)dst) == 0x03)))
        {
            if(src != dst)
            {
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"mov %"+registerNAME(src)+", %"+registerNAME(dst));
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"mov "+registerNAME(dst)+", "+registerNAME(src));
                        break;
                }
            }
        }
        else
        {
            if((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)dst) == 0x0100000000)
            {
                pushRegSave();
                if(fstore->registerStatus(rax) != 0)
                    saveRegister(rax);
                if(fstore->registerStatus(rdx) != 0)
                    saveRegister(rdx);
                x86_64::mov((uint64_t)0,rdx);
                x86_64::mov(src,rax);
                writeMSR(dst);
                restoreRegisters();
            }
            else if((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)src) == 0x0100000000)
            {
                pushRegSave();
                if(fstore->registerStatus(rax) != 0 && dst != rax)
                    saveRegister(rax);
                if(fstore->registerStatus(rdx) != 0 && dst != rdx)
                    saveRegister(rdx);
                readMSR(src);
                x86_64::mov(rax,dst);
                restoreRegisters();
            }
        }
    }

    void mov(location src, location dst)
    {
        switch(syntax)
        {
            case(SYNTAX_GAS):
                code->push_back(getIndent()+"mov "+src.expr()+", "+dst.expr());
                break;
            case(SYNTAX_INTEL):
                code->push_back(getIndent()+"mov "+dst.expr()+", "+src.expr());
                break;
        }
    }

    void mov(uint64_t src, __register__ dst)
    {
        using enum __register__;
        if((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)dst) == 0x00)
        {
            if(src != 0)
            {
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"mov $"+intToString(src)+", %"+registerNAME(dst));
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"mov "+registerNAME(dst)+", "+intToString(src));
                        break;
                }
            }
            else
            {
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"xor %"+registerNAME(dst)+", %"+registerNAME(dst));
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"xor "+registerNAME(dst)+", "+registerNAME(dst));
                        break;
                }
            }
        }
        else if((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)dst) == 0x0100000000)
        {
            x86_64::mov(src,rax);
            x86_64::mov(rax,dst);
        }
        else
        {
            std::cout << "register type: " << (uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)dst) << std::endl;
        }
    }

    void mov(location src, __register__ dst)
    {
        switch(syntax)
        {
            case(SYNTAX_GAS):
                code->push_back(getIndent()+"mov "+src.expr()+", %"+registerNAME(dst));
                break;
            case(SYNTAX_INTEL):
                code->push_back(getIndent()+"mov "+registerNAME(dst)+", "+src.expr());
                break;
        }
    }

    void mov(__register__ src, location dst)
    {
        switch(syntax)
        {
            case(SYNTAX_GAS):
                code->push_back(getIndent()+"mov %"+registerNAME(src)+", "+dst.expr());
                break;
            case(SYNTAX_INTEL):
                code->push_back(getIndent()+"mov "+dst.expr()+", "+registerNAME(src));
                break;
        }
    }

    void mov(variable* src, __register__ dst)
    {
        if(src->storage == storageType::REGISTER)
        {
            x86_64::mov(src->reg,dst);
        }
        else if(src->storage == storageType::MEMORY)
        {
            x86_64::mov(location(__register__::rsp,src->offset),dst);
        }
        else if(src->storage == storageType::MEMORY_ABSOLUTE)
        {
            x86_64::mov(location(src->offset),dst);
        }
        else if(src->storage == storageType::IMMEDIATE)
        {
            x86_64::mov(src->immediateValue,dst);
        }
    }
    void mov(__register__ src, variable* dst)
    {
        if(dst->storage == storageType::REGISTER)
        {
            x86_64::mov(src,dst->reg);
        }
        else if(dst->storage == storageType::MEMORY)
        {
            x86_64::mov(src,location(__register__::rsp,dst->offset));
        }
        else if(dst->storage == storageType::MEMORY_ABSOLUTE)
        {
            x86_64::mov(src,location(dst->offset));
        }
        else if(dst->storage == storageType::IMMEDIATE)
        {
            std::cout << "blub storage type immediate (1)" << std::endl;
        }
    }

    void mov(variable* src, variable* dst)
    {
        if(options::dalog){
            std::cout 
            << "[mov]"
            << "(" <<COLOR_TYPE<<src->dataType->name <<' '<<COLOR_VAR<< src->name <<COLOR_RESET<< ") " << location(src).expr()
            << " ==> " 
            << "(" <<COLOR_TYPE<<dst->dataType->name <<' '<<COLOR_VAR<< dst->name <<COLOR_RESET<< ") " << location(dst).expr()
            << std::endl;
        }
        if(dst->storage == storageType::REGISTER && src->storage == storageType::REGISTER)
            x86_64::mov(src->reg,dst->reg);
        else if(src->storage == storageType::MEMORY && dst->storage == storageType::REGISTER)
        {
            if(dst->dataType->size <= 2)
            {
                x86_64::mov(_0,dst->reg);
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"mov "+location(src).expr()+", %"+registerNAME(dst->reg,dst->dataType->size));
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"mov "+registerNAME(dst->reg,dst->dataType->size)+", "+location(src).expr());
                        break;
                }
            }
            else
            {
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"mov "+location(src).expr()+", %"+registerNAME(dst->reg,dst->dataType->size));
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"mov "+registerNAME(dst->reg,dst->dataType->size)+", "+location(src).expr());
                        break;
                }
            }
        }
        else if(src->storage == storageType::REGISTER && dst->storage == storageType::MEMORY)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"mov %"+registerNAME(src->reg,src->dataType->size)+", "+location(dst).expr());
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"mov "+location(dst).expr()+", "+registerNAME(src->reg,src->dataType->size));
                    break;
            }
        }
        else if(dst->storage == storageType::MEMORY && src->storage == storageType::MEMORY)
            x86_64::mov(location(src),location(dst));
        else if(dst->storage == storageType::MEMORY_ABSOLUTE && src->storage == storageType::REGISTER)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"mov %"+registerNAME(src->reg,src->dataType->size)+", "+location(dst).expr());
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"mov "+location(dst).expr()+", "+registerNAME(src->reg,src->dataType->size));
                    break;
            }
        }
        else if(src->storage == storageType::MEMORY_ABSOLUTE && dst->storage == storageType::REGISTER)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"mov "+location(src).expr()+", %"+registerNAME(dst->reg,dst->dataType->size));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"mov "+registerNAME(dst->reg,dst->dataType->size)+", %"+location(src).expr());
                    break;
            }
        }
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
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"mov "+location(src).expr()+", %"+registerNAME(reg,src->dataType->size));
                    code->push_back(getIndent()+"mov %"+registerNAME(reg,dst->dataType->size)+", "+location(dst).expr());
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", "+location(src).expr());
                    code->push_back(getIndent()+"mov "+location(dst).expr()+", "+registerNAME(reg,dst->dataType->size));
                    break;
            }
            if(rInvalid);
                restoreRegisters();
        }
        else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::MEMORY_ABSOLUTE)
        {
            if(src->immediateValue <= 0xFFFFFFFF)
            {
                //imm32,imm16,imm8
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"mov $"+intToString(src->immediateValue)+", "+location(dst).expr());
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"mov "+location(dst).expr()+", "+intToString(src->immediateValue));
                        break;
                }
            }
            else
            {
                if(dst->dataType->size < 8)
                    return error::genericError(0x8664003);
                //imm64
                //this uses movabs under GAS, mov under any other assembler
                __register__ reg = fstore->getFreeRegister();
                bool rInvalid = false;
                pushRegSave();
                if(reg == __register__::invalid)
                {
                    rInvalid = true;
                    reg = __register__::rax;
                    saveRegister(reg);
                }
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"movabsq $"+intToString(src->immediateValue)+", %"+registerNAME(reg,8));
                        code->push_back(getIndent()+"mov %"+registerNAME(reg,8)+", "+location(dst).expr());
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"mov "+registerNAME(reg,8)+", "+intToString(src->immediateValue));
                        code->push_back(getIndent()+"mov "+location(dst).expr()+", "+registerNAME(reg,8));
                        break;
                }
                restoreRegisters();
                popRegSave();
            }
        }
        else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::MEMORY)
        {
            switch(dst->dataType->size)
            {
                case(1):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movb $"+intToString(src->immediateValue)+", "+location(dst).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov byte "+location(dst).expr()+", "+intToString(src->immediateValue));
                            break;
                    }
                    break;
                case(2):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movw $"+intToString(src->immediateValue)+", "+location(dst).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov word "+location(dst).expr()+", "+intToString(src->immediateValue));
                            break;
                    }
                    break;
                case(4):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movl $"+intToString(src->immediateValue)+", "+location(dst).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov dword "+location(dst).expr()+", "+intToString(src->immediateValue));
                            break;
                    }
                    break;
                case(8):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movq $"+intToString(src->immediateValue)+", "+location(dst).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov qword "+location(dst).expr()+", "+intToString(src->immediateValue));
                            break;
                    }
                    break;
            }
        }
        else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::REGISTER)
        {
            if(dst->dataType->size < src->dataType->size)
            {
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"xor %"+registerNAME(dst->reg)+", %"+registerNAME(dst->reg));
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"xor "+registerNAME(dst->reg)+", "+registerNAME(dst->reg));
                        break;
                }
            }
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"mov $"+intToString(src->immediateValue)+", %"+registerNAME(dst->reg,dst->dataType->size));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"mov "+registerNAME(dst->reg,dst->dataType->size)+", "+intToString(src->immediateValue));
                    break;
            }
        }
        else if(src->storage == storageType::SYMBOL && dst->storage == storageType::REGISTER)
        {
            if(dst->dataType->size <= 2)
            {
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"xor %"+registerNAME(dst->reg)+", %"+registerNAME(dst->reg));
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"xor "+registerNAME(dst->reg)+", "+registerNAME(dst->reg));
                        break;
                }
            }
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"mov "+src->symbol+", %"+registerNAME(dst->reg,dst->dataType->size));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"mov "+registerNAME(dst->reg,dst->dataType->size)+", "+src->symbol);
                    break;
            }
        }
        else if(src->storage == storageType::SYMBOL_ADDR && dst->storage == storageType::REGISTER)
        {
            if(dst->dataType->size < src->dataType->size)
            {
                switch(syntax)
                {
                    case(SYNTAX_GAS):
                        code->push_back(getIndent()+"xor %"+registerNAME(dst->reg)+", %"+registerNAME(dst->reg));
                        break;
                    case(SYNTAX_INTEL):
                        code->push_back(getIndent()+"xor "+registerNAME(dst->reg)+", "+registerNAME(dst->reg));
                        break;
                }
            }
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"mov $"+src->symbol+", %"+registerNAME(dst->reg,dst->dataType->size));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"mov "+registerNAME(dst->reg,dst->dataType->size)+", offset "+src->symbol+"");
                    break;
            }
        }
        else if(src->storage == storageType::SYMBOL && dst->storage == storageType::SYMBOL)
        {
            __register__ reg = fstore->getFreeRegister();
            pushRegSave();
            if(reg == __register__::invalid)
            {
                reg = __register__::rax;
                saveRegister(reg);
            }
            switch(dst->dataType->size)
            {
                case(1):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movb "+src->symbol+", %"+registerNAME(reg,src->dataType->size));
                            code->push_back(getIndent()+"movb %"+registerNAME(reg,dst->dataType->size)+", "+dst->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", byte "+src->symbol);
                            code->push_back(getIndent()+"mov byte "+dst->symbol+", "+registerNAME(reg,dst->dataType->size));
                            break;
                    }
                    break;
                case(2):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movw "+src->symbol+", %"+registerNAME(reg,src->dataType->size));
                            code->push_back(getIndent()+"movw %"+registerNAME(reg,dst->dataType->size)+", "+dst->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", word "+src->symbol);
                            code->push_back(getIndent()+"mov word "+dst->symbol+", "+registerNAME(reg,dst->dataType->size));
                            break;
                    }
                    break;
                case(4):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movl "+src->symbol+", %"+registerNAME(reg,src->dataType->size));
                            code->push_back(getIndent()+"movl %"+registerNAME(reg,dst->dataType->size)+", "+dst->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", dword "+src->symbol);
                            code->push_back(getIndent()+"mov dword "+dst->symbol+", "+registerNAME(reg,dst->dataType->size));
                            break;
                    }
                    break;
                case(8):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movq "+src->symbol+", %"+registerNAME(reg,src->dataType->size));
                            code->push_back(getIndent()+"movq %"+registerNAME(reg,dst->dataType->size)+", "+dst->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", qword "+src->symbol);
                            code->push_back(getIndent()+"mov qword "+dst->symbol+", "+registerNAME(reg,dst->dataType->size));
                            break;
                    }
                    break;
            }
            restoreRegisters();
            popRegSave();
        }
        else if(src->storage == storageType::SYMBOL && dst->storage == storageType::MEMORY)
        {
            __register__ reg = fstore->getFreeRegister();
            pushRegSave();
            if(reg == __register__::invalid)
            {
                reg = __register__::rax;
                saveRegister(reg);
            }
            switch(dst->dataType->size)
            {
                case(1):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"mov "+src->symbol+", %"+registerNAME(reg,src->dataType->size));
                            x86_64::mov(_0,dst->reg);
                            code->push_back(getIndent()+"mov %"+registerNAME(reg,dst->dataType->size)+", "+location(dst).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", "+src->symbol);
                            x86_64::mov(_0,dst->reg);
                            code->push_back(getIndent()+"mov "+location(dst).expr()+", "+registerNAME(reg,dst->dataType->size));
                            break;
                    }
                    break;
                case(2):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"mov "+src->symbol+", %"+registerNAME(reg,src->dataType->size));
                            x86_64::mov(_0,dst->reg);
                            code->push_back(getIndent()+"mov %"+registerNAME(reg,dst->dataType->size)+", "+location(dst).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", "+src->symbol);
                            x86_64::mov(_0,dst->reg);
                            code->push_back(getIndent()+"mov "+location(dst).expr()+", "+registerNAME(reg,dst->dataType->size));
                            break;
                    }
                    break;
                case(4):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"mov "+src->symbol+", %"+registerNAME(reg,src->dataType->size));
                            code->push_back(getIndent()+"mov %"+registerNAME(reg,dst->dataType->size)+", "+location(dst).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", "+src->symbol);
                            code->push_back(getIndent()+"mov "+location(dst).expr()+", "+registerNAME(reg,dst->dataType->size));
                            break;
                    }
                    break;
                case(8):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"mov "+src->symbol+", %"+registerNAME(reg,src->dataType->size));
                            code->push_back(getIndent()+"mov %"+registerNAME(reg,dst->dataType->size)+", "+location(dst).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,src->dataType->size)+", "+src->symbol);
                            code->push_back(getIndent()+"mov "+location(dst).expr()+", "+registerNAME(reg,dst->dataType->size));
                            break;
                    }
                    break;
            }
            restoreRegisters();
            popRegSave();
        }
        else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::SYMBOL)
        {
            switch(dst->dataType->size)
            {
                case(1):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movb $"+intToString(src->immediateValue)+", "+dst->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov byte "+dst->symbol+", "+intToString(src->immediateValue));
                            break;
                    }
                    break;
                case(2):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movw $"+intToString(src->immediateValue)+", "+dst->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov word "+dst->symbol+", "+intToString(src->immediateValue));
                            break;
                    }
                    break;
                case(4):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movd $"+intToString(src->immediateValue)+", "+dst->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov dword "+dst->symbol+", "+intToString(src->immediateValue));
                            break;
                    }
                    break;
                case(8):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movq $"+intToString(src->immediateValue)+", "+dst->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov qword ptr "+dst->symbol+", "+intToString(src->immediateValue));
                            break;
                    }
                    break;
            }
        }
        else if(src->storage == storageType::REGISTER && dst->storage == storageType::SYMBOL)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"mov %"+registerNAME(src->reg,dst->dataType->size)+", "+dst->symbol);
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"mov "+dst->symbol+", "+registerNAME(src->reg,dst->dataType->size));
                    break;
            }
        }
        else
        {
            std::cout << "well, ig im dumb LMFAO" << std::endl;
            std::cout << "src: " << std::endl;
            printVariable(src);
            std::cout << "dst: " << std::endl;
            printVariable(dst);
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
        x86_64::mov(src,reg);
        x86_64::mov(reg,dst);
        if(rInvalid);
            restoreRegisters();
    }

    void mov(variable* src,location dst)
    {
        if(src->storage == storageType::REGISTER)
            x86_64::mov(src->reg,dst);
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
            x86_64::mov(src,reg);
            x86_64::mov(reg,dst);
            if(rInvalid);
                restoreRegisters();
        }
    }

    void mov(location src,variable* dst)
    {
        if(dst->storage == storageType::REGISTER)
            x86_64::mov(src,dst->reg);
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
            x86_64::mov(src,reg);
            x86_64::mov(reg,dst);
            if(rInvalid);
                restoreRegisters();
        }
    }
}
