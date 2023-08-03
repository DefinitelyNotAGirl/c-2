/*
 * Created Date: Thursday August 3rd 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 3rd 2023 1:02:05 pm
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

#include "master.h"
#include <util.h>
#include <stack>

std::stack<std::vector<__register__>> savedRegisters;
std::stack<std::vector<uint64_t>> savedRegisterOffsets;
std::vector<std::string>* code = nullptr;
functionStorage* fstore = nullptr;

void pushRegSave()
{
    savedRegisters.push(std::vector<__register__>());
    savedRegisterOffsets.push(std::vector<uint64_t>());
}

void popRegSave()
{
    savedRegisters.pop();
    savedRegisterOffsets.pop();
}

void saveRegister(__register__ reg)
{
    savedRegisters.top().push_back(reg);
    switch(reg)
    {
        case(__register__::rax):
        case(__register__::rbx):
        case(__register__::rcx):
        case(__register__::rdx):
        case(__register__::rsi):
        case(__register__::rdi):
        case(__register__::rbp):
        case(__register__::rsp):
        case(__register__::r8):  
        case(__register__::r9):  
        case(__register__::r10): 
        case(__register__::r11): 
        case(__register__::r12): 
        case(__register__::r13): 
        case(__register__::r14): 
        case(__register__::r15): 
        case(__register__::rip): 
        case(__register__::cr0): 
        case(__register__::cr1): 
        case(__register__::cr2): 
        case(__register__::cr3): 
        case(__register__::cr4): 
        case(__register__::cr5): 
        case(__register__::cr6): 
        case(__register__::cr7): 
        case(__register__::cr8): 
        case(__register__::cr9): 
        case(__register__::cr10):
        case(__register__::cr11):
        case(__register__::cr12):
        case(__register__::cr13):
        case(__register__::cr14):
        case(__register__::cr15):
            mov(reg,location(__register__::rsp,fstore->stackSize));
            savedRegisterOffsets.top().push_back(fstore->stackSize);
            fstore->stackSize+=8;
            break;
    }
}
void restoreRegisters()
{
    for(uint64_t i = 0;i<savedRegisters.top().size();i++)
        mov(location(__register__::rsp,savedRegisterOffsets.top()[i]),savedRegisters.top()[i]);
    savedRegisters.top().clear();
    savedRegisterOffsets.top().clear();
}

variable* call_SysVAmd64_SysVAmd64(function* func,std::vector<variable*> args)
{
    //call from SysV to SysV
    code = &currentScope->func->code;

    if(func->isPrimitive)
        return primitiveCall(func,args);
    else
        std::cout << func->name << " is not primitive" << std::endl;

    //save registers
    pushRegSave();
    saveRegister(__register__::rax);
    saveRegister(__register__::rdi);
    saveRegister(__register__::rsi);
    saveRegister(__register__::rdx);
    saveRegister(__register__::rcx);
    saveRegister(__register__::r8);
    saveRegister(__register__::r9);
    saveRegister(__register__::r10);
    saveRegister(__register__::r11);

    //pass arguments
    functionStorage* calleeStore = new functionStorage;
    calleeStore->ABI = fstore->ABI;
    for(variable* arg : args)
    {
        variable* pArg = new variable;
        pArg->dataType = arg->dataType;
        calleeStore->setArgumentStorage(pArg);
        mov(arg,pArg);
    }
    //call
    currentScope->func->code.push_back(getIndent()+"call "+func->symbol);

    //restore registers
    restoreRegisters();
    popRegSave();

    variable* ret = new variable;
    ret->storage = storageType::REGISTER;
    ret->reg = __register__::rax;
    return ret;
}

variable* call_SysVAmd64_Microsoftx64(function* func,std::vector<variable*> args)
{
    //call from SysV to MS
    return nullptr;
}

variable* call_Microsoftx64_SysVAmd64(function* func,std::vector<variable*> args)
{
    //call from MS to SysV
    return nullptr;
}

variable* call_Microsoftx64_Microsoftx64(function* func,std::vector<variable*> args)
{
    //call from MS to MS
    return nullptr;
}

variable* call(function* func,std::vector<variable*> args)
{
    scope* sc = currentScope;
    fstore = sc->fstore;
    if(sc->t != scopeType::FUNCTION)
    {
        std::cout << "bruh :skull:" << std::endl;
    }
    switch(sc->fstore->ABI)
    {
        case(1):
            switch(func->fstore->ABI)
            {
                case(1):
                    return call_SysVAmd64_SysVAmd64(func,args);
                case(2):
                    return call_SysVAmd64_Microsoftx64(func,args);
                default:
                    std::cout << "??????????????????????????" << std::endl;
            }
            break;
        case(2):
            switch(func->fstore->ABI)
            {
                case(1):
                    return call_Microsoftx64_SysVAmd64(func,args);
                case(2):
                    return call_Microsoftx64_Microsoftx64(func,args);
            }
            break;
    }
    return nullptr;
}