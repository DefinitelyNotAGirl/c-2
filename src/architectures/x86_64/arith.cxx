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
    //
    //
    // add
    //
    //

    void add(__register__ src, __register__ dst)
    {
        code->push_back(getIndent()+"add %"+registerNAME(src)+", %"+registerNAME(dst));
    }
    void add(uint64_t src, __register__ dst)
    {
        if(src > 1)
            code->push_back(getIndent()+"add $"+std::to_string(src)+", %"+registerNAME(dst));
        else if(src == 1)
            code->push_back(getIndent()+"inc %"+registerNAME(dst));
    }
    void add(variable* dst, variable* a, variable* b)
    {
        using enum __register__;
        pushRegSave();
        __register__ dstReg = dst->reg;
        if(dst->storage != storageType::REGISTER)
        {
            dstReg = fstore->getFreeRegister();
            if(dstReg == invalid)
            {
                dstReg = rax;
                saveRegister(dstReg);
            }
            x86_64::mov(dst,dstReg);
        }
        x86_64::mov(a,dstReg);
        switch(b->storage)
        {
            case(storageType::REGISTER):
                code->push_back(getIndent()+"add %"+registerNAME(b->reg)+", %"+registerNAME(dstReg));
                break;
            case(storageType::MEMORY):
                code->push_back(getIndent()+"add "+location(b->reg,b->offset).expr()+", %"+registerNAME(dstReg));
                break;
            case(storageType::MEMORY_ABSOLUTE):
                code->push_back(getIndent()+"add "+location(b->offset).expr()+", %"+registerNAME(dstReg));
                break;
            case(storageType::IMMEDIATE):
                code->push_back(getIndent()+"add $"+std::to_string(b->immediateValue)+", %"+registerNAME(dstReg));
                break;
        }

        restoreRegisters();
        popRegSave();
    }

    //
    //
    // sub
    //
    //

    void sub(__register__ src, __register__ dst)
    {
        code->push_back(getIndent()+"sub %"+registerNAME(src)+", %"+registerNAME(dst));
    }
    void sub(uint64_t src, __register__ dst)
    {
        if(src > 1)
            code->push_back(getIndent()+"sub $"+std::to_string(src)+", %"+registerNAME(dst));
        else if(src == 1)
            code->push_back(getIndent()+"dec %"+registerNAME(dst));
    }
    void sub(variable* dst, variable* a, variable* b)
    {
        using enum __register__;
        pushRegSave();
        __register__ dstReg = dst->reg;
        if(dst->storage != storageType::REGISTER)
        {
            dstReg = fstore->getFreeRegister();
            if(dstReg == invalid)
            {
                dstReg = rax;
                saveRegister(dstReg);
            }
            x86_64::mov(dst,dstReg);
        }
        x86_64::mov(a,dstReg);
        switch(b->storage)
        {
            case(storageType::REGISTER):
                code->push_back(getIndent()+"sub %"+registerNAME(b->reg)+", %"+registerNAME(dstReg));
                break;
            case(storageType::MEMORY):
                code->push_back(getIndent()+"sub "+location(b->reg,b->offset).expr()+", %"+registerNAME(dstReg));
                break;
            case(storageType::MEMORY_ABSOLUTE):
                code->push_back(getIndent()+"sub "+location(b->offset).expr()+", %"+registerNAME(dstReg));
                break;
            case(storageType::IMMEDIATE):
                code->push_back(getIndent()+"sub $"+std::to_string(b->immediateValue)+", %"+registerNAME(dstReg));
                break;
        }

        restoreRegisters();
        popRegSave();
    }

    //
    //
    // mul
    //
    //

    void mul(variable* dst, variable* right, variable* left)
    {
        using enum __register__;
        pushRegSave();
        bool savedRDX = false;
        if(fstore->registerStatus(rdx) != 0)
        {
            saveRegister(rdx);
            savedRDX = true;
        }
        fstore->registerStatus(rdx,1);
        __register__ rreg = right->reg;
        if(!(right->storage == storageType::REGISTER && right->reg == rax))
        {
            if(fstore->registerStatus(rax) != 0)
                saveRegister(rax);
            x86_64::mov(right,rax);
            rreg = rax;
        }
        else if(right->storage == storageType::REGISTER)
        {
            rreg = right->reg;
        }

        switch(left->storage)
        {
            case(storageType::REGISTER):
                code->push_back(getIndent()+"mul %"+registerNAME(left->reg));
                break;
            case(storageType::IMMEDIATE):
                switch(left->immediateValue)
                {
                    case(0):
                        x86_64::mov((uint64_t)0,rreg);
                        break;
                    case(1):
                        //multiplication by 1 can simply be ignored
                        break;
                    case(2):
                        code->push_back(getIndent()+"shl %"+registerNAME(rreg)+", $1");
                        break;
                    case(4):
                        code->push_back(getIndent()+"shl %"+registerNAME(rreg)+", $2");
                        break;
                    case(8):
                        code->push_back(getIndent()+"shl %"+registerNAME(rreg)+", $3");
                        break;
                    case(16):
                        code->push_back(getIndent()+"shl %"+registerNAME(rreg)+", $4");
                        break;
                    case(32):
                        code->push_back(getIndent()+"shl %"+registerNAME(rreg)+", $5");
                        break;
                    case(64):
                        code->push_back(getIndent()+"shl %"+registerNAME(rreg)+", $6");
                        break;
                    default:
                        __register__ tmp = fstore->getFreeRegister();
                        if(tmp == invalid)
                        {
                            tmp = r8;
                            if(fstore->registerStatus(r8) != 0)
                                saveRegister(r8);
                        }
                        x86_64::mov(left->immediateValue,tmp);
                        code->push_back(getIndent()+"mul %"+registerNAME(tmp));
                }
                break;
            case(storageType::MEMORY):
                code->push_back(getIndent()+"mul "+location(rsp,left->offset).expr());
                break;
            case(storageType::MEMORY_ABSOLUTE):
                code->push_back(getIndent()+"mul "+location(left->offset).expr());
                break;
        }

        x86_64::mov(rreg,dst);
        restoreRegisters();
        popRegSave();
        if(!savedRDX)
            fstore->registerStatus(rdx,0);
    }

    void mod(variable* dst, variable* right, variable* left)
    {
        using enum __register__;
        pushRegSave();
        bool savedRDX = false;
        if(fstore->registerStatus(rdx) != 0)
        {
            saveRegister(rdx);
            savedRDX = true;
        }
        x86_64::mov((uint64_t)0,rdx);
        fstore->registerStatus(rdx,1);
        __register__ rreg = right->reg;
        if(!(right->storage == storageType::REGISTER && right->reg == rax))
        {
            if(fstore->registerStatus(rax) != 0)
                saveRegister(rax);
            x86_64::mov(right,rax);
            rreg = rax;
        }
        else if(right->storage == storageType::REGISTER)
        {
            rreg = right->reg;
        }

        switch(left->storage)
        {
            case(storageType::REGISTER):
                code->push_back(getIndent()+"div %"+registerNAME(left->reg));
                break;
            case(storageType::IMMEDIATE):
                switch(left->immediateValue)
                {
                    case(0):
                        //error, division by zero
                        break;
                    case(1):
                        //division by 1 can simply be ignored
                        break;
                    default:
                        __register__ tmp = fstore->getFreeRegister();
                        if(options::ddebug)
                        {
                            std::cout << "free register: " << registerNAME(tmp) << std::endl;
                            std::cout << "status: " << fstore->registerStatus(tmp) << std::endl;
                        }
                        if(tmp == invalid)
                        {
                            tmp = r8;
                            if(fstore->registerStatus(r8) != 0)
                                saveRegister(r8);
                        }
                        x86_64::mov(left->immediateValue,tmp);
                        code->push_back(getIndent()+"div %"+registerNAME(tmp));
                }
                break;
            case(storageType::MEMORY):
                code->push_back(getIndent()+"div "+location(rsp,left->offset).expr());
                break;
            case(storageType::MEMORY_ABSOLUTE):
                code->push_back(getIndent()+"div "+location(left->offset).expr());
                break;
        }

        x86_64::mov(rdx,dst);
        restoreRegisters();
        popRegSave();
        if(!savedRDX)
            fstore->registerStatus(rdx,0);
    }

    //
    //
    // div
    //
    //

    void div(__register__ left, __register__ right)
    {
    }
    void div(__register__ left, uint64_t right)
    {
    }
    void div(variable* dst, variable* right, variable* left)
    {
    }

    //
    //
    // inc/dec
    //
    //
    void inc(variable* target)
    {
        switch(target->storage)
        {
            case(storageType::REGISTER):
                code->push_back(getIndent()+"inc %"+registerNAME(target->reg));
                break;
            case(storageType::MEMORY):
                code->push_back(getIndent()+"inc "+location(target->reg,target->offset).expr());
                break;
            case(storageType::MEMORY_ABSOLUTE):
                code->push_back(getIndent()+"inc "+location(target->offset).expr());
                break;
            default:
                std::cout << "tf this shit?" << std::endl;
                break;
        }
    }

    void dec(variable* target)
    {
        switch(target->storage)
        {
            case(storageType::REGISTER):
                code->push_back(getIndent()+"dec %"+registerNAME(target->reg));
                break;
            case(storageType::MEMORY):
                code->push_back(getIndent()+"dec "+location(target->reg,target->offset).expr());
                break;
            case(storageType::MEMORY_ABSOLUTE):
                code->push_back(getIndent()+"dec "+location(target->offset).expr());
                break;
            default:
                std::cout << "tf this shit?" << std::endl;
                break;
        }
    }
}
