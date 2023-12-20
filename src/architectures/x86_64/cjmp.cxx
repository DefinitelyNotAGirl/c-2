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
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp %"+registerNAME(a->reg)+",%"+registerNAME(b->reg));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+registerNAME(b->reg)+","+registerNAME(a->reg));
                    break;
            }
        }
        else if(b->storage == storageType::REGISTER && a->storage == storageType::MEMORY)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp %"+location(a->reg,a->offset).expr()+","+registerNAME(b->reg));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+registerNAME(b->reg)+","+location(a->reg,a->offset).expr());
                    break;
            }
        }
        else if(a->storage == storageType::REGISTER && b->storage == storageType::MEMORY)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp %"+registerNAME(a->reg)+","+location(b).expr());
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+location(b).expr()+","+registerNAME(a->reg));
                    break;
            }
        }
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
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp %"+registerNAME(reg)+","+location(b->reg,b->offset).expr());
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+location(b->reg,b->offset).expr()+","+registerNAME(reg));
                    break;
            }
            if(rInvalid);
                restoreRegisters();
        }
        else if(b->storage == storageType::MEMORY_ABSOLUTE && a->storage == storageType::REGISTER)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp %"+registerNAME(a->reg)+","+location(b->offset).expr());
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+location(b->offset).expr()+","+registerNAME(a->reg));
                    break;
            }
        }
        else if(a->storage == storageType::MEMORY_ABSOLUTE && b->storage == storageType::REGISTER)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp "+location(a->offset).expr()+",%"+registerNAME(b->reg));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+registerNAME(b->reg)+","+location(a->offset).expr());
                    break;
            }
        }
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
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp %"+registerNAME(reg)+",%"+registerNAME(b->reg));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+registerNAME(b->reg)+","+registerNAME(reg));
                    break;
            }
            if(rInvalid);
                restoreRegisters();
        }
        else if(b->storage == storageType::MEMORY_ABSOLUTE && a->storage == storageType::IMMEDIATE)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp $"+intToString(a->immediateValue)+","+location(b->offset).expr());
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+location(b->offset).expr()+",$"+intToString(a->immediateValue));
                    break;
            }
        }
        else if(b->storage == storageType::MEMORY && a->storage == storageType::IMMEDIATE)
        {
            std::string instr = "cmp";
            switch(syntax)
            {
                case(SYNTAX_GAS):
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
                    code->push_back(getIndent()+instr+" $"+intToString(a->immediateValue)+","+location(b).expr());
                    break;
                case(SYNTAX_INTEL):
                    switch(b->dataType->size)
                    {
                        case(1):
                            instr = " byte ";
                            break;
                        case(2):
                            instr = " word ";
                            break;
                        case(4):
                            instr = " dword ";
                            break;
                        case(8):
                            instr = " qword ";
                            break;
                    }
                    code->push_back(getIndent()+"cmp "+location(b).expr()+","+instr+"ptr "+intToString(a->immediateValue));
                    break;
            }
        }
        else if(b->storage == storageType::REGISTER && a->storage == storageType::IMMEDIATE)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp $"+intToString(a->immediateValue)+",%"+registerNAME(b->reg));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+registerNAME(b->reg)+",$"+intToString(a->immediateValue));
                    break;
            }
        }
        else if(b->storage == storageType::SYMBOL && a->storage == storageType::REGISTER)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp "+b->symbol+",%"+registerNAME(a->reg,a->dataType->size));
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+registerNAME(a->reg,a->dataType->size)+","+b->symbol);
                    break;
            }
        }
        else if(a->storage == storageType::SYMBOL && b->storage == storageType::REGISTER)
        {
            switch(syntax)
            {
                case(SYNTAX_GAS):
                    code->push_back(getIndent()+"cmp %"+registerNAME(b->reg,b->dataType->size)+","+a->symbol);
                    break;
                case(SYNTAX_INTEL):
                    code->push_back(getIndent()+"cmp "+a->symbol+","+registerNAME(b->reg,b->dataType->size));
                    break;
            }
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
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movb "+b->symbol+", %"+registerNAME(reg));
                            code->push_back(getIndent()+"cmpb %"+registerNAME(reg)+", "+a->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg)+", byte "+b->symbol);
                            code->push_back(getIndent()+"cmp byte "+a->symbol+", "+registerNAME(reg));
                            break;
                    }
                    break;
                case(2):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movw "+b->symbol+", %"+registerNAME(reg));
                            code->push_back(getIndent()+"cmpw %"+registerNAME(reg)+", "+a->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg)+", word "+b->symbol);
                            code->push_back(getIndent()+"cmp word "+a->symbol+", "+registerNAME(reg));
                            break;
                    }
                    break;
                case(4):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movl "+b->symbol+", %"+registerNAME(reg));
                            code->push_back(getIndent()+"cmpl %"+registerNAME(reg)+", "+a->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg)+", dword "+b->symbol);
                            code->push_back(getIndent()+"cmp dword "+a->symbol+", "+registerNAME(reg));
                            break;
                    }
                    break;
                case(8):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"movb "+b->symbol+", %"+registerNAME(reg));
                            code->push_back(getIndent()+"cmpb %"+registerNAME(reg)+", "+a->symbol);
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg)+", qword "+b->symbol);
                            code->push_back(getIndent()+"cmp qword "+a->symbol+", "+registerNAME(reg));
                            break;
                    }
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
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            x86_64::mov(getImmediateVariable(0),reg);
                            code->push_back(getIndent()+"mov "+b->symbol+", %"+registerNAME(reg,b->dataType->size));
                            code->push_back(getIndent()+"cmp %"+registerNAME(reg,b->dataType->size)+", "+location(a).expr());
                            break;
                        case(SYNTAX_INTEL):
                            x86_64::mov(getImmediateVariable(0),reg);
                            code->push_back(getIndent()+"mov "+registerNAME(reg,b->dataType->size)+", %"+b->symbol);
                            code->push_back(getIndent()+"cmp "+location(a).expr()+", "+registerNAME(reg,b->dataType->size));
                            break;
                    }
                    break;
                case(2):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            x86_64::mov(getImmediateVariable(0),reg);
                            code->push_back(getIndent()+"mov "+b->symbol+", %"+registerNAME(reg,b->dataType->size));
                            code->push_back(getIndent()+"cmp %"+registerNAME(reg,b->dataType->size)+", "+location(a).expr());
                            break;
                        case(SYNTAX_INTEL):
                            x86_64::mov(getImmediateVariable(0),reg);
                            code->push_back(getIndent()+"mov "+registerNAME(reg,b->dataType->size)+", %"+b->symbol);
                            code->push_back(getIndent()+"cmp "+location(a).expr()+", "+registerNAME(reg,b->dataType->size));
                            break;
                    }
                    break;
                case(4):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"mov "+b->symbol+", %"+registerNAME(reg,b->dataType->size));
                            code->push_back(getIndent()+"cmp %"+registerNAME(reg,b->dataType->size)+", "+location(a).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,b->dataType->size)+", %"+b->symbol);
                            code->push_back(getIndent()+"cmp "+location(a).expr()+", "+registerNAME(reg,b->dataType->size));
                            break;
                    }
                    break;
                case(8):
                    switch(syntax)
                    {
                        case(SYNTAX_GAS):
                            code->push_back(getIndent()+"mov "+b->symbol+", %"+registerNAME(reg,b->dataType->size));
                            code->push_back(getIndent()+"cmp %"+registerNAME(reg,b->dataType->size)+", "+location(a).expr());
                            break;
                        case(SYNTAX_INTEL):
                            code->push_back(getIndent()+"mov "+registerNAME(reg,b->dataType->size)+", %"+b->symbol);
                            code->push_back(getIndent()+"cmp "+location(a).expr()+", "+registerNAME(reg,b->dataType->size));
                            break;
                    }
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