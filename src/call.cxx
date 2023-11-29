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
#include <codegen.h>

std::stack<std::vector<__register__>> savedRegisters;
std::stack<std::vector<uint64_t>> savedRegisterOffsets;
std::stack<std::vector<uint64_t>> savedRegisterDebug;
uint64_t regSaveDebugCounter = 0;
std::vector<std::string>* code = nullptr;
functionStorage* fstore = nullptr;
function* codeGenFunc = nullptr;
std::string getFunctionExpression(function* f, bool showVariableNames = false);

void pushRegSave()
{
    savedRegisters.push(std::vector<__register__>());
    savedRegisterOffsets.push(std::vector<uint64_t>());
    if(asmDebugComments)
        savedRegisterDebug.push(std::vector<uint64_t>());
}

void popRegSave()
{
    savedRegisters.pop();
    savedRegisterOffsets.pop();
    if(asmDebugComments)
        savedRegisterDebug.pop();
}

void saveRegister(__register__ reg)
{
    //std::cout << "SAVE!!!!!!!!!!!!!!!!!!!!!!!!! " << registerNAME(reg) << std::endl;
    savedRegisters.top().push_back(reg);
    mov(reg,location(StackPointer,fstore->stackOffset));
    savedRegisterOffsets.top().push_back(fstore->stackOffset);
    if(asmDebugComments)
    {
        code->back().append(" # save @"+std::to_string(regSaveDebugCounter));
        savedRegisterDebug.top().push_back(regSaveDebugCounter++);
    }
    //std::cout << "BLUB BLUB SIZE: " <<std::hex<< ((((uint64_t)reg)&BITMASK_REGISTER_SIZE) / 0x1000000) << "("<<registerNAME(reg)<<")" << std::endl;
    fstore->stackOffset+=((((uint64_t)reg)&BITMASK_REGISTER_SIZE) / 0x1000000);
    if(fstore->stackOffset > fstore->stackSize)
        fstore->stackSize = fstore->stackOffset;
}
void restoreRegisters()
{
    //std::cout << "RESTORE!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    for(uint64_t i = 0;i<savedRegisters.top().size();i++)
    {
        mov(location(StackPointer,savedRegisterOffsets.top()[i]),savedRegisters.top()[i]);
        fstore->stackOffset-=((((uint64_t)savedRegisters.top()[i])&BITMASK_REGISTER_SIZE) / 0x1000000);
        if(asmDebugComments)
            code->back().append(" # restore @"+std::to_string(savedRegisterDebug.top()[i]));
    }
    savedRegisters.top().clear();
    savedRegisterOffsets.top().clear();
    #ifdef ASMDEBUG
        savedRegisterDebug.top().clear();
    #endif
}

void jump(function* func, std::string symbol)
{
    code = &func->code;
    jmp(symbol);
}
void jump(std::string symbol)
{
    jmp(symbol);
}

void placeSymbol(function* func,std::string symbol)
{
    CodePlaceSymbol(symbol);
}
void placeSymbol(std::string symbol)
{
    CodePlaceSymbol(symbol);
}

void codeGenUpdateFuction()
{
    switch(currentScope->t)
    {
        case(scopeType::FUNCTION):
        case(scopeType::CONDITIONAL_BLOCK):
        case(scopeType::LOGICAL):
        case(scopeType::DUMMY):
            codeGenFunc = currentScope->func;
            code = &currentScope->func->code;
            fstore = currentScope->fstore;
            break;
    }
}

variable* call(function* func,std::vector<variable*>& args)
{
    //std::cout << "func: " <<std::hex<< (void*)func << std::endl;
    if(options::ddebug)
        std::cout << "calling: " << getFunctionExpression(func) << std::endl;
    if(func->isPrimitive)
        return primitiveCall(func,args);
    //else
    //    std::cout << func->name << " is not primitive" << std::endl;
    //std::cout << "cs->func->abi: " << std::hex << (void*)currentScope->func->abi << std::endl;
    //std::cout << "cs->func->abi->name: " << currentScope->func->abi->name << std::endl;
    //std::cout << "cs->func->abi->call: " << std::hex << (void*)currentScope->func->abi->call << std::endl;
    return currentScope->func->abi->call(func,args);
}
