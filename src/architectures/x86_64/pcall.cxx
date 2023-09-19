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

#include <compiler.h>
#include <function.h>
#include <codegen.h>
#include <mangling.h>

namespace x86_64
{
    void cmp(variable*,variable*);
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
                case(primitiveOP::assign):
                    //std::cout << "assign!" << std::endl;
                    x86_64::mov(args[1],ret);
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
                        ret->name = getNewName();
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
                        ret->name = getNewName();
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
                        ret->name = getNewName();
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
                        ret->name = getNewName();
                        fstore->setStorage(func,ret);
                        x86_64::add(ret,args[0],args[1]);
                    }
                    break;
                case(primitiveOP::sub):
                    //std::cout << "sub!" << std::endl;
                    if(func->primitiveInPlace)
                    {
                        x86_64::add(args[0],args[0],args[1]);
                    }
                    else
                    {
                        ret->dataType = args[0]->dataType;
                        ret->name = getNewName();
                        fstore->setStorage(func,ret);
                        x86_64::add(ret,args[0],args[1]);
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
                        ret->name = getNewName();
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
                        ret->name = getNewName();
                        fstore->setStorage(func,ret);
                        x86_64::mov(args[0],ret);
                        x86_64::dec(ret);
                    }
                    break;
                case(primitiveOP::equal):
                {
                    x86_64::cmp(args[0],args[1]);
                    boolRetCJmp = currentArch->je;
                    goto BOOLRET;
                }
                case(primitiveOP::LessEqual):
                {
                    x86_64::cmp(args[0],args[1]);
                    boolRetCJmp = currentArch->jle;
                    goto BOOLRET;
                }
                case(primitiveOP::Less):
                {
                    x86_64::cmp(args[0],args[1]);
                    boolRetCJmp = currentArch->jl;
                    goto BOOLRET;
                }
                case(primitiveOP::GreaterEqual):
                {
                    x86_64::cmp(args[0],args[1]);
                    boolRetCJmp = currentArch->jge;
                    goto BOOLRET;
                }
                case(primitiveOP::Greater):
                {
                    x86_64::cmp(args[0],args[1]);
                    boolRetCJmp = currentArch->jge;
                    goto BOOLRET;
                }
                case(primitiveOP::NotEqual):
                {
                    x86_64::cmp(args[0],args[1]);
                    boolRetCJmp = currentArch->jne;
                    goto BOOLRET;
                }
                default:
                    std::cout << "blub ("<<func->name<<")" << std::endl;
                    break;
                {
                    BOOLRET:;
                    std::vector<std::string>* codeBlock = new std::vector<std::string>;
                    std::string entSymbol = currentScope->name+CPE2_SYMBOL_SCOPE_SEP"boolRetConditional"+std::to_string(currentScope->booleanReturnCounter++);
                    std::string retSymbol = entSymbol+CPE2_SYMBOL_SCOPE_SEP"reentry";
                    currentScope->extraCodeBlocks.push_back(codeBlock);
                    std::vector<std::string>* currentCode = code;
                    //
                    ret->dataType = defaultBooleanType;
                    ret->name = getNewName();
                    fstore->setStorage(codeGenFunc,ret);
                    //generate conditional jump code
                    boolRetCJmp(entSymbol);
                    //generate non conditional code
                    x86_64::mov(getImmediateVariable(0),ret);
                    //set return symbol
                    placeSymbol(retSymbol);
                    //generate conditinal code
                    code = codeBlock;
                    placeSymbol(entSymbol);
                    x86_64::mov(getImmediateVariable(1),ret);
                    x86_64::jmp(retSymbol);
                    //restore compiler state
                    code = currentCode;
                    break;
                }
            }
        }
        else
        {
            std::cout << "blub (2)" << std::endl;
        }
        return ret;
    }
}