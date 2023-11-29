/*
 * Created Date: Sunday September 17th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday September 17th 2023 10:37:09 pm
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

namespace x86_64
{
    void cmp(variable* a, variable* b)
    {
        if(b->storage == storageType::REGISTER && a->storage == storageType::REGISTER)
            code->push_back(getIndent()+"cmp %"+registerNAME(a->reg)+",%"+registerNAME(b->reg));
        else if(b->storage == storageType::REGISTER && a->storage == storageType::MEMORY)
            code->push_back(getIndent()+"cmp %"+location(a->reg,a->offset).expr()+","+registerNAME(b->reg));
        else if(a->storage == storageType::REGISTER && b->storage == storageType::MEMORY)
            code->push_back(getIndent()+"cmp %"+registerNAME(a->reg)+","+location(b).expr());
        else if(b->storage == storageType::MEMORY && a->storage == storageType::MEMORY)
        {
            //cant compare directly
            //need intermediate register
            __register__ reg = fstore->getFreeRegister();
            bool rInvalid = false;
            if(reg == __register__::invalid)
            {
                rInvalid = true;
                reg = __register__::rax;
                saveRegister(reg);
            }
            x86_64::mov(a,reg);
            code->push_back(getIndent()+"cmp %"+registerNAME(reg)+","+location(b->reg,b->offset).expr());
            if(rInvalid);
                restoreRegisters();
        }
        else if(b->storage == storageType::MEMORY_ABSOLUTE && a->storage == storageType::REGISTER)
            code->push_back(getIndent()+"cmp %"+registerNAME(a->reg)+","+location(b->offset).expr());
        else if(a->storage == storageType::MEMORY_ABSOLUTE && b->storage == storageType::REGISTER)
            code->push_back(getIndent()+"cmp "+location(a->offset).expr()+",%"+registerNAME(b->reg));
        else if(a->storage == storageType::MEMORY_ABSOLUTE && b->storage == storageType::MEMORY_ABSOLUTE)
        {
            //cant cmp directly
            //need intermediate register
            __register__ reg = fstore->getFreeRegister();
            bool rInvalid = false;
            if(reg == __register__::invalid)
            {
                rInvalid = true;
                reg = __register__::rax;
                saveRegister(reg);
            }
            x86_64::mov(a,reg);
            code->push_back(getIndent()+"cmp %"+registerNAME(reg)+",%"+registerNAME(b->reg));
            if(rInvalid);
                restoreRegisters();
        }
        else if(b->storage == storageType::MEMORY_ABSOLUTE && a->storage == storageType::IMMEDIATE)
        {
            code->push_back(getIndent()+"cmp $"+std::to_string(a->immediateValue)+","+location(b->offset).expr());
        }
        else if(b->storage == storageType::MEMORY && a->storage == storageType::IMMEDIATE)
        {
            std::string instr = "cmp";
            switch(b->dataType->size)
            {
                case(1):
                    instr = "cmpb";
                    break;
                case(2):
                    instr = "cmpw";
                    break;
                case(4):
                    instr = "cmpd";
                    break;
                case(8):
                    instr = "cmpq";
                    break;
            }
            code->push_back(getIndent()+instr+" $"+std::to_string(a->immediateValue)+","+location(b).expr());
        }
        else if(b->storage == storageType::REGISTER && a->storage == storageType::IMMEDIATE)
        {
            code->push_back(getIndent()+"cmp $"+std::to_string(a->immediateValue)+",%"+registerNAME(b->reg));
        }
        else if(b->storage == storageType::SYMBOL && a->storage == storageType::REGISTER)
        {
            code->push_back(getIndent()+"cmp "+b->symbol+",%"+registerNAME(a->reg,a->dataType->size));
        }
        else if(a->storage == storageType::SYMBOL && b->storage == storageType::REGISTER)
        {
            code->push_back(getIndent()+"cmp %"+registerNAME(b->reg,b->dataType->size)+","+a->symbol);
        }
        else if(b->storage == storageType::IMMEDIATE)
        {
            std::cout << "b: " << std::endl;
            printVariable(b);
            std::cout << "ERROR: cant put immediate on left side of cmp!" << std::endl;
        }
        else if(b->storage == storageType::SYMBOL && a->storage == storageType::SYMBOL)
        {
            __register__ reg = fstore->getFreeRegister();
            pushRegSave();
            if(reg == __register__::invalid)
            {
                reg = __register__::rax;
                saveRegister(reg);
            }
            switch(b->dataType->size)
            {
                case(1):
                    code->push_back(getIndent()+"movb "+b->symbol+", %"+registerNAME(reg));
                    code->push_back(getIndent()+"cmpb %"+registerNAME(reg)+", "+a->symbol);
                    break;
                case(2):
                    code->push_back(getIndent()+"movw "+b->symbol+", %"+registerNAME(reg));
                    code->push_back(getIndent()+"cmpw %"+registerNAME(reg)+", "+a->symbol);
                    break;
                case(4):
                    code->push_back(getIndent()+"movd "+b->symbol+", %"+registerNAME(reg));
                    code->push_back(getIndent()+"cmpd %"+registerNAME(reg)+", "+a->symbol);
                    break;
                case(8):
                    code->push_back(getIndent()+"movq "+b->symbol+", %"+registerNAME(reg));
                    code->push_back(getIndent()+"cmpq %"+registerNAME(reg)+", "+a->symbol);
                    break;
            }
            restoreRegisters();
            popRegSave();
        }
        else if(b->storage == storageType::SYMBOL && a->storage == storageType::MEMORY)
        {
            __register__ reg = fstore->getFreeRegister();
            pushRegSave();
            if(reg == __register__::invalid)
            {
                reg = __register__::rax;
                saveRegister(reg);
            }
            switch(b->dataType->size)
            {
                case(1):
                    x86_64::mov(getImmediateVariable(0),reg);
                    code->push_back(getIndent()+"mov "+b->symbol+", %"+registerNAME(reg,b->dataType->size));
                    code->push_back(getIndent()+"cmp %"+registerNAME(reg,b->dataType->size)+", "+location(a).expr());
                    break;
                case(2):
                    x86_64::mov(getImmediateVariable(0),reg);
                    code->push_back(getIndent()+"mov "+b->symbol+", %"+registerNAME(reg,b->dataType->size));
                    code->push_back(getIndent()+"cmp %"+registerNAME(reg,b->dataType->size)+", "+location(a).expr());
                    break;
                case(4):
                    code->push_back(getIndent()+"mov "+b->symbol+", %"+registerNAME(reg,b->dataType->size));
                    code->push_back(getIndent()+"cmp %"+registerNAME(reg,b->dataType->size)+", "+location(a).expr());
                    break;
                case(8):
                    code->push_back(getIndent()+"mov "+b->symbol+", %"+registerNAME(reg,b->dataType->size));
                    code->push_back(getIndent()+"cmp %"+registerNAME(reg,b->dataType->size)+", "+location(a).expr());
                    break;
            }
            restoreRegisters();
            popRegSave();
        }
        else
        {
            std::cout << "well, ig im dumb LMFAO" << std::endl;
            std::cout << "a: " << (uint64_t)a->storage << std::endl;
            std::cout << "b: " << (uint64_t)b->storage << std::endl;
        }
    }
    void jnz(std::string symbol){code->push_back(getIndent()+"jnz "+symbol);}
    void jne(std::string symbol){code->push_back(getIndent()+"jne "+symbol);}
    void jno(std::string symbol){code->push_back(getIndent()+"jno "+symbol);}
    void jns(std::string symbol){code->push_back(getIndent()+"jns "+symbol);}
    void js(std::string symbol){code->push_back(getIndent()+"js "+symbol);}
    void jz(std::string symbol){code->push_back(getIndent()+"jz "+symbol);}
    void je(std::string symbol){code->push_back(getIndent()+"je "+symbol);}
    void jo(std::string symbol){code->push_back(getIndent()+"jo "+symbol);}
    void jb(std::string symbol){code->push_back(getIndent()+"jb "+symbol);}
    void jnae(std::string symbol){code->push_back(getIndent()+"jnae "+symbol);}
    void jc(std::string symbol){code->push_back(getIndent()+"jc "+symbol);}
    void jnb(std::string symbol){code->push_back(getIndent()+"jnb "+symbol);}
    void jae(std::string symbol){code->push_back(getIndent()+"jae "+symbol);}
    void jnc(std::string symbol){code->push_back(getIndent()+"jnc "+symbol);}
    void jbe(std::string symbol){code->push_back(getIndent()+"jbe "+symbol);}
    void jna(std::string symbol){code->push_back(getIndent()+"jna "+symbol);}
    void jnbe(std::string symbol){code->push_back(getIndent()+"jnbe "+symbol);}
    void ja(std::string symbol){code->push_back(getIndent()+"ja "+symbol);}
    void jnge(std::string symbol){code->push_back(getIndent()+"jnge "+symbol);}
    void jl(std::string symbol){code->push_back(getIndent()+"jl "+symbol);}
    void jge(std::string symbol){code->push_back(getIndent()+"jge "+symbol);}
    void jnl(std::string symbol){code->push_back(getIndent()+"jnl "+symbol);}
    void jle(std::string symbol){code->push_back(getIndent()+"jle "+symbol);}
    void jng(std::string symbol){code->push_back(getIndent()+"jng "+symbol);}
    void jnle(std::string symbol){code->push_back(getIndent()+"jnle "+symbol);}
    void jg(std::string symbol){code->push_back(getIndent()+"jg "+symbol);}
    void jp(std::string symbol){code->push_back(getIndent()+"jp "+symbol);}
    void jpe(std::string symbol){code->push_back(getIndent()+"jpe "+symbol);}
    void jnp(std::string symbol){code->push_back(getIndent()+"jnp "+symbol);}
    void jpo(std::string symbol){code->push_back(getIndent()+"jpo "+symbol);}
    void jcxz(std::string symbol){code->push_back(getIndent()+"jcxz "+symbol);}
    void jecxz(std::string symbol){code->push_back(getIndent()+"jecxz "+symbol);}
}