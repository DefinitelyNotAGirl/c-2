/*
 * Created Date: Thursday August 3rd 2023
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

namespace x86_64
{
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
        x86_64::mov(getx86MSR(src),rcx);
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
        x86_64::mov(getx86MSR(dst),rcx);
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
                code->push_back(getIndent()+"mov %"+registerNAME(src)+", %"+registerNAME(dst));
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
        code->push_back(getIndent()+"mov "+src.expr()+", "+dst.expr());
    }

    void mov(uint64_t src, __register__ dst)
    {
        using enum __register__;
        if((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)dst) == 0x00)
        {
            if(src != 0)
                code->push_back(getIndent()+"mov $"+std::to_string(src)+", %"+registerNAME(dst));
            else
                code->push_back(getIndent()+"xor %"+registerNAME(dst)+", %"+registerNAME(dst));
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
        if(dst->storage == storageType::REGISTER && src->storage == storageType::REGISTER)
            x86_64::mov(src->reg,dst->reg);
        else if(dst->storage == storageType::REGISTER && src->storage == storageType::MEMORY)
            x86_64::mov(location(__register__::rsp,src->offset),dst->reg);
        else if(src->storage == storageType::REGISTER && dst->storage == storageType::MEMORY)
            x86_64::mov(src->reg,location(__register__::rsp,dst->offset));
        else if(dst->storage == storageType::MEMORY && src->storage == storageType::MEMORY)
            x86_64::mov(location(__register__::rsp,src->offset),location(__register__::rsp,dst->offset));
        else if(dst->storage == storageType::MEMORY_ABSOLUTE && src->storage == storageType::REGISTER)
            x86_64::mov(src->reg,location(dst->offset));
        else if(src->storage == storageType::MEMORY_ABSOLUTE && dst->storage == storageType::REGISTER)
            x86_64::mov(location(src->offset),dst->reg);
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
            x86_64::mov(src,reg);
            x86_64::mov(reg,dst);
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
            x86_64::mov(src,reg);
            x86_64::mov(reg,dst);
            if(rInvalid);
                restoreRegisters();
        }
        else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::MEMORY)
        {
            switch(dst->dataType->size)
            {
                case(1):
                    code->push_back(getIndent()+"movb $"+std::to_string(src->immediateValue)+", "+location(dst->reg,dst->offset).expr());
                    break;
                case(2):
                    code->push_back(getIndent()+"movw $"+std::to_string(src->immediateValue)+", "+location(dst->reg,dst->offset).expr());
                    break;
                case(4):
                    code->push_back(getIndent()+"movl $"+std::to_string(src->immediateValue)+", "+location(dst->reg,dst->offset).expr());
                    break;
                case(8):
                    code->push_back(getIndent()+"movq $"+std::to_string(src->immediateValue)+", "+location(dst->reg,dst->offset).expr());
                    break;
            }
        }
        else if(src->storage == storageType::IMMEDIATE && dst->storage == storageType::REGISTER)
        {
            x86_64::mov(src->immediateValue,dst->reg);
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
