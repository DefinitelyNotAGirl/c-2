/*
 * Created Date: Wednesday September 13th 2023
 * Author: Lilith
 * -----
 * Last Modified: Friday September 15th 2023 8:41:50 pm
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
#include <mangling.h>

#define constructor __attribute__ ((constructor))

#include <codegen.h>

namespace __ABI__{
using enum __register__;

static ABI* abi = nullptr;

static std::list<__register__> integerArgPass = {rdi,rsi,r8,r9,r10,r11,r12};
static std::list<__register__> floatArgPass = {xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7};

static void genProlouge(std::vector<std::string>& lines, scope* sc)
{
    std::vector<std::string> tlines;
    code = &tlines;
    sc->fstore->stackSize = roundUp(sc->fstore->stackSize, 16);
    pushRegSave();
    scope* cs = currentScope;
    currentScope = cs->parent;
    lines.push_back(getIndent()+sc->func->symbol+":");
    currentScope = cs;
    for(__register__ r : abi->nonVolatile)
        if(sc->fstore->registerStatus(r) == 1)
            saveRegister(r);
    code = &lines;
    sub(sc->fstore->stackSize,StackPointer);
    for(std::string& i : tlines)
        lines.push_back(i);
}

static void genEpilouge(std::vector<std::string>& lines, scope* sc)
{
    code = &lines;
    lines.push_back(getIndent()+sc->name+CPE2_SYMBOL_SCOPE_SEP+"epilogue:");
    restoreRegisters();
    popRegSave();
    add(sc->fstore->stackSize,StackPointer);
    lines.push_back(getIndent()+"ret");
}

static void preCall(function* func)
{
}

static void postCall(function* func)
{
}

static void setArgStorages(function* func,std::vector<variable*>& args)
{
    std::list<__register__> IAP = integerArgPass;
    std::list<__register__> FAP = floatArgPass;
    for(variable* I : args)
    {
        switch(I->dataType->regMode)
        {
            case(1):
                //store in integer register if avaiable
                if(IAP.size() > 0 && I->dataType->size <= 8)
                {
                    I->reg = IAP.front();
                    I->storage = storageType::REGISTER;
                    func->fstore->registerStatus(IAP.front(),1);
                    IAP.pop_front();
                }
                else
                    goto STOREONSTACK;
                break;
            case(2):
                //store in floating point register if avaiable
                if(FAP.size() > 0 && I->dataType->size <= 8)
                {
                    I->reg = FAP.front();
                    I->storage = storageType::REGISTER;
                    func->fstore->registerStatus(FAP.front(),1);
                    FAP.pop_front();
                }
                else
                    goto STOREONSTACK;
                break;
            case(0):
                //store on stack
                STOREONSTACK:;
                I->reg = StackPointer;
                I->storage = storageType::MEMORY;
                I->offset = func->fstore->stackSize;
                func->fstore->stackSize+=I->dataType->size;
                break;
        }
    }
}

static void moveArguments(function* func,std::vector<variable*>& args)
{
    std::list<__register__> IAP = integerArgPass;
    std::list<__register__> FAP = floatArgPass;
    variable* nvar = nullptr;
    for(variable* I : args)
    {
        nvar = (variable*)malloc(sizeof(variable));
        memcpy(nvar,I,sizeof(variable));
        switch(I->dataType->regMode)
        {
            case(1):
                //store in integer register if avaiable
                if(IAP.size() > 0 && I->dataType->size <= 8)
                {
                    nvar->reg = IAP.front();
                    nvar->storage = storageType::REGISTER;
                    IAP.pop_front();
                }
                else
                    goto STOREONSTACK;
                break;
            case(2):
                //store in floating point register if avaiable
                if(FAP.size() > 0 && I->dataType->size <= 8)
                {
                    nvar->reg = FAP.front();
                    nvar->storage = storageType::REGISTER;
                    FAP.pop_front();
                }
                else
                    goto STOREONSTACK;
                break;
            case(0):
                //store on stack
                STOREONSTACK:;
                nvar->reg = StackPointer;
                nvar->storage = storageType::MEMORY;
                nvar->offset = func->fstore->stackSize;
                func->fstore->stackSize+=nvar->dataType->size;
                break;
        }
        mov(I,nvar);
    }
}

static variable* call(function* func,std::vector<variable*>& args)
{
    pushRegSave();
    if(!func->noReturn)
        for(__register__ I : func->abi->VolatileRegisters)
            if(fstore->registerStatus(I) == 1)
                saveRegister(I);
    func->abi->moveArguments(func,args);
    func->abi->preCall(func);
    func->abi->instrCall(func);
    if(!func->noReturn)
        func->abi->postCall(func);
    if(!func->noReturn)
        restoreRegisters();
    popRegSave();

    variable* ret = new variable;
    ret->storage = storageType::REGISTER;
    ret->reg = func->abi->integerReturn;
    ret->dataType = func->returnType;
    return ret;
}

static void instrCall(function* func)
{
    code->push_back(getIndent()+"call "+func->symbol);
}

constructor static void init()
{
    abi = new ABI;
    abi->name = "cpe2x64";
    abi->moveArguments = &moveArguments;
    abi->setArgStorages = &setArgStorages;
    abi->genProlouge = &genProlouge;
    abi->genEpilouge = &genEpilouge;
    abi->call = &__ABI__::call;
    abi->preCall = &preCall;
    abi->postCall = &postCall;
    abi->instrCall = &instrCall;
    abi->floatReturn = xmm0;
    abi->integerReturn = rax;

    //add volatile registers
    for(__register__ I : registers)
    {
        if(EXPR_GETBIT_00((uint64_t)I) != 1)
            goto SKIPVREG;
        switch((uint64_t)(BITMASK_REGISTER_TYPE & (uint64_t)I))
        {
            case(0x0100000000):
            case(0x0200000000):
            case(0x0300000000):
            case(0x0400000000):
            case(0x0600000000):
                goto SKIPVREG;
        }
        if(I == StackPointer)
            goto SKIPVREG;
        abi->VolatileRegisters.push_back(I);
        SKIPVREG:;
    }
    //add non volatile registers

    ABIs.push_back(abi);//add our ABI to the global ABI list
}

}//namespace __ABI__