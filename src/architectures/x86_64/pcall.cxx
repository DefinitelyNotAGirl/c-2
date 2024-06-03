/*
 * Created Date: Thursday August 3rd 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday May 22nd 2024 11:30:22 am
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
#include <function.h>
#include <codegen.h>
#include <error.h>
#include <mangling.h>

void updateCurrentScope(scope* sc);

namespace x86_64
{
    uint64_t cmp(variable*,variable*);
    void jmp(std::string symbol);
    variable* primitiveCall(function* func,std::vector<variable*> args)
    {
        using enum __register__;
        variable* ret;
        void(*boolRetCJmp)(std::string symbol) = nullptr;
        if(func->primitiveInPlace)
        {
            ret = args[0];
        }
        else
        {
            ret = new variable;
        }
        if(!func->primitiveFloat)
        {
            switch(func->op)
            {
                case(primitiveOP::PRINTCHAR):
                    csys->printChar(args[0]);
                    break;
                case(primitiveOP::PRINTSTR):
                    csys->printStr(args[0]);
                    break;
                case(primitiveOP::Interrupt):
                    //std::cout << "interrupt!" << std::endl;
                    if(args[0]->storage == storageType::IMMEDIATE)
                        code->push_back(getIndent()+"int $"+std::to_string(args[0]->immediateValue));
                    else
                        error::genericError(0x8664008);
                    break;
                case(primitiveOP::CPUid):
                    //std::cout << "cpuid!" << std::endl;
                    setANB(16);
                    x86_64::mov(args[0],rax);
                    code->push_back(getIndent()+"cpuid");
                    popANB();
                    break;
                case(primitiveOP::SYSCALL):
                    //std::cout << "syscall!" << std::endl;
                    code->push_back(getIndent()+"syscall");
                    break;
                case(primitiveOP::assign):
                    //std::cout << "assign!" << std::endl;
                    if(currentScope->t != scopeType::GLOBAL && currentScope->t != scopeType::NAMESPACE)
                    {
                        x86_64::mov(args[1],ret);
                    }
                    else
                    {
                        if(args[1]->storage == storageType::IMMEDIATE)
                        {
                            DataCode.push_back(ret->symbol+":");
                            switch(ret->dataType->size)
                            {
                                case(1):
                                    DataCode.push_back("\t.byte "+intToString(args[1]->immediateValue));
                                    break;
                                case(2):
                                    DataCode.push_back("\t.word "+intToString(args[1]->immediateValue));
                                    break;
                                case(4):
                                    DataCode.push_back("\t.int "+intToString(args[1]->immediateValue));
                                    break;
                                case(8):
                                    DataCode.push_back("\t.quad "+intToString(args[1]->immediateValue));
                                    break;
                            }
                            ret->storage = storageType::SYMBOL;
                        }
                        else if(args[1]->storage == storageType::SYMBOL)
                        {
                            DataCode.push_back(ret->symbol+":");
                            switch(ret->dataType->size)
                            {
                                case(1):
                                    DataCode.push_back("\t.byte "+args[1]->symbol);
                                    break;
                                case(2):
                                    DataCode.push_back("\t.word "+args[1]->symbol);
                                    break;
                                case(4):
                                    DataCode.push_back("\t.int "+args[1]->symbol);
                                    break;
                                case(8):
                                    DataCode.push_back("\t.quad "+args[1]->symbol);
                                    break;
                            }
                            ret->storage = storageType::SYMBOL;
                        }
                        else if(args[1]->storage == storageType::SYMBOL_ADDR)
                        {
                            DataCode.push_back(ret->symbol+":");
                            switch(args[1]->dataType->size)
                            {
                                case(1):
                                    DataCode.push_back("\t.byte "+args[1]->symbol);
                                    break;
                                case(2):
                                    DataCode.push_back("\t.word "+args[1]->symbol);
                                    break;
                                case(4):
                                    DataCode.push_back("\t.int "+args[1]->symbol);
                                    break;
                                case(8):
                                    DataCode.push_back("\t.quad "+args[1]->symbol);
                                    break;
                            }
                            ret->storage = storageType::SYMBOL;
                        }
                        else
                        {
                            error::genericError(0x8664002);
                            printVariable(ret);
                        }
                    }
                    break;
                case(primitiveOP::mul):
                    //std::cout << "multiply!" << std::endl;
                    if(func->primitiveInPlace)
                    {
                        x86_64::mul(args[0],args[0],args[1]);
                    }
                    else
                    {
                        ret->dataType = args[0]->dataType;
                        ret->name = getNewVariableName();
                        fstore->setStorage(func,ret);
                        x86_64::mul(ret,args[0],args[1]);
                    }
                    break;
                case(primitiveOP::div):
                    //std::cout << "divide!" << std::endl;
                    if(func->primitiveInPlace)
                    {
                        x86_64::div(args[0],args[0],args[1]);
                    }
                    else
                    {
                        ret->dataType = args[0]->dataType;
                        ret->name = getNewVariableName();
                        fstore->setStorage(func,ret);
                        x86_64::div(ret,args[0],args[1]);
                    }
                    break;
                case(primitiveOP::mod):
                    //std::cout << "modulo!" << std::endl;
                    if(func->primitiveInPlace)
                    {
                        x86_64::mod(args[0],args[0],args[1]);
                    }
                    else
                    {
                        ret->dataType = args[0]->dataType;
                        ret->name = getNewVariableName();
                        fstore->setStorage(func,ret);
                        x86_64::mod(ret,args[0],args[1]);
                    }
                    break;
                case(primitiveOP::add):
                    //std::cout << "add!" << std::endl;
                    if(func->primitiveInPlace)
                    {
                        x86_64::add(args[0],args[0],args[1]);
                    }
                    else
                    {
                        ret->dataType = args[0]->dataType;
                        ret->name = getNewVariableName();
                        fstore->setStorage(func,ret);
                        x86_64::add(ret,args[0],args[1]);
                    }
                    break;
                case(primitiveOP::sub):
                    //std::cout << "sub!" << std::endl;
                    if(func->primitiveInPlace)
                    {
                        x86_64::sub(args[0],args[0],args[1]);
                    }
                    else
                    {
                        ret->dataType = args[0]->dataType;
                        ret->name = getNewVariableName();
                        fstore->setStorage(func,ret);
                        x86_64::sub(ret,args[0],args[1]);
                    }
                    break;
                case(primitiveOP::Inc):
                    //std::cout << "inc!" << std::endl;
                    if(func->primitiveInPlace)
                    {
                        x86_64::inc(args[0]);
                    }
                    else
                    {
                        ret->dataType = args[0]->dataType;
                        ret->name = getNewVariableName();
                        fstore->setStorage(func,ret);
                        x86_64::mov(args[0],ret);
                        x86_64::inc(ret);
                    }
                    break;
                case(primitiveOP::Dec):
                    //std::cout << "dec!" << std::endl;
                    if(func->primitiveInPlace)
                    {
                        x86_64::dec(args[0]);
                    }
                    else
                    {
                        ret->dataType = args[0]->dataType;
                        ret->name = getNewVariableName();
                        fstore->setStorage(func,ret);
                        x86_64::mov(args[0],ret);
                        x86_64::dec(ret);
                    }
                    break;
                case(primitiveOP::Index):
                    __register__ pointerReg;
                    ret = new variable;
                    if(args[0]->storage == storageType::REGISTER)
                    {
                        pointerReg = args[0]->reg;
                        pointerRegSet:;
                        ret->name = getNewVariableName();
                        ret->reg = pointerReg;
                        ret->storage = storageType::MEMORY;
                        ret->dataType = args[0]->dataType->valueType;
                        if(args[1]->storage == storageType::REGISTER)
                        {
                            ret->offsetType = storageType::REGISTER;
                            ret->offsetReg = args[1]->reg;
                        }
                        else if(args[1]->storage == storageType::IMMEDIATE)
                        {
                            ret->offsetType = storageType::IMMEDIATE;
                            ret->offset = args[1]->immediateValue*ret->dataType->size;
                        }
                        else
                            error::genericError(0x8664001);
                    }
                    else
                    {
                        x86_64::mov(args[0],rcx);
                        pointerReg = rcx;
                        goto pointerRegSet;
                    }
                    break;
                case(primitiveOP::equal):
                {
					uint64_t result = x86_64::cmp(args[1],args[0]);
                    switch(result)
                    {
                        case(0):
                        {
                            //success
							jumplastcondition = currentArch->je;
                            break;
                        }
                        case(1):
                        {
                            //cant use immediate as 2nd argument to cmp instruction
                            //flip arguments and execute reverse instruction
                        	x86_64::cmp(args[0],args[1]);
							jumplastcondition = currentArch->je;
                            break;
                        }
                        default: {errorCompilerBug;break;}
					}
					break;
                }
                case(primitiveOP::LessEqual):
                {
					uint64_t result = x86_64::cmp(args[1],args[0]);
                    switch(result)
                    {
                        case(0):
                        {
                            //success
							jumplastcondition = currentArch->jle;
                            break;
                        }
                        case(1):
                        {
                            //cant use immediate as 2nd argument to cmp instruction
                            //flip arguments and execute reverse instruction
                        	x86_64::cmp(args[0],args[1]);
							jumplastcondition = currentArch->jge;
                            break;
                        }
                        default: {errorCompilerBug;break;}
					}
					break;
                }
                case(primitiveOP::Less):
                {
					uint64_t result = x86_64::cmp(args[1],args[0]);
                    switch(result)
                    {
                        case(0):
                        {
                            //success
							jumplastcondition = currentArch->jl;
                            break;
                        }
                        case(1):
                        {
                            //cant use immediate as 2nd argument to cmp instruction
                            //flip arguments and execute reverse instruction
                        	x86_64::cmp(args[0],args[1]);
							jumplastcondition = currentArch->jg;
                            break;
                        }
                        default: {errorCompilerBug;break;}
					}
					break;
                }
                case(primitiveOP::GreaterEqual):
                {
					uint64_t result = x86_64::cmp(args[1],args[0]);
                    switch(result)
                    {
                        case(0):
                        {
                            //success
							jumplastcondition = currentArch->jge;
                            break;
                        }
                        case(1):
                        {
                            //cant use immediate as 2nd argument to cmp instruction
                            //flip arguments and execute reverse instruction
                        	x86_64::cmp(args[0],args[1]);
							jumplastcondition = currentArch->jl;
                            break;
                        }
                        default: {errorCompilerBug;break;}
					}
					break;
                }
                case(primitiveOP::Greater):
                {
					uint64_t result = x86_64::cmp(args[1],args[0]);
                    switch(result)
                    {
                        case(0):
                        {
                            //success
							jumplastcondition = currentArch->jg;
                            break;
                        }
                        case(1):
                        {
                            //cant use immediate as 2nd argument to cmp instruction
                            //flip arguments and execute reverse instruction
                        	x86_64::cmp(args[0],args[1]);
							jumplastcondition = currentArch->jl;
                            break;
                        }
                        default: {errorCompilerBug;break;}
					}
					break;
                }
                case(primitiveOP::NotEqual):
                {
                    uint64_t result = x86_64::cmp(args[1],args[0]);
                    switch(result)
                    {
                        case(0):
                        {
                            //success
							jumplastcondition = currentArch->jne;
                            break;
                        }
                        case(1):
                        {
                            //cant use immediate as 2nd argument to cmp instruction
                            //flip arguments and execute reverse instruction
                        	x86_64::cmp(args[0],args[1]);
							jumplastcondition = currentArch->jne;
                            break;
                        }
                        default: {errorCompilerBug;break;}
					}
					break;
                }
                default:
                    errorCompilerBug;
                    break;
            }
        }
        else
        {
            std::cout << "blub (2)" << std::endl;
        }
        return ret;
    }
}