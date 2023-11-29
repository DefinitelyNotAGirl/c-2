/*
 * Created Date: Saturday September 30th 2023
 * Author: Lilith
 * -----
 * Last Modified: Saturday September 30th 2023 11:33:37 pm
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
#include <mangling.h>
#define constructor __attribute__ ((constructor))

namespace linux
{
    void printStr(variable* str)
    {
        using enum __register__;
        pushRegSave();
        if(fstore->registerStatus(rax) == 1)saveRegister(rax);//arguments to syscall
        if(fstore->registerStatus(rdi) == 1)saveRegister(rdi);//arguments to syscall
        if(fstore->registerStatus(rsi) == 1)saveRegister(rsi);//arguments to syscall
        if(fstore->registerStatus(rdx) == 1)saveRegister(rdx);//arguments to syscall
        if(fstore->registerStatus(rcx) == 1)saveRegister(rcx);//trashed by syscall
        if(fstore->registerStatus(r11) == 1)saveRegister(r11);//trashed by syscall
        // debug mov(getImmediateVariable(0),rax);//(clear for now, performance reasons)
        mov(str,rsi);//char buffer
        mov(getImmediateVariable(0),rdx);
        //mov(getImmediateVariable(13),rdx);
        // debug mov(getImmediateVariable(0),rdi);
        variable* __str = new variable;
        __str->storage = storageType::MEMORY;
        __str->reg = rsi;
        __str->offsetType = storageType::REGISTER;
        __str->offsetReg = rdx;
        __str->dataType = str->dataType->valueType;
        variable _rdx;
        _rdx.storage = storageType::REGISTER;
        _rdx.reg = rdx;
        variable _rax;
        _rax.storage = storageType::REGISTER;
        _rax.reg = rax;
        variable _rdi;
        _rdi.storage = storageType::REGISTER;
        _rdi.reg = rdi;
        // debug mov(getImmediateVariable(18446744073709551615),rdx);
        // debug mov(getImmediateVariable(0),rdx);
        std::string symb = currentScope->name+CPE2_SYMBOL_SCOPE_SEP"strlen"+std::to_string(currentScope->strlenCounter++);
        placeSymbol(symb);
        inc(&_rdx);
        cmp(getImmediateVariable(0),__str);
        // debug code->push_back(getIndent()+"movb (%rsi,%rdx), %dil");
        jne(symb);
        mov(getImmediateVariable(1),rax);//syscall function (write)
        mov(&_rax,&_rdi);
        code->push_back(getIndent()+"syscall");
        restoreRegisters();
        popRegSave();
    }

    void printChar(variable* c)
    {
        using enum __register__;
        pushRegSave();
        if(fstore->registerStatus(rax) == 1)saveRegister(rax);//arguments to syscall
        if(fstore->registerStatus(rdi) == 1)saveRegister(rdi);//arguments to syscall
        if(fstore->registerStatus(rsi) == 1)saveRegister(rsi);//arguments to syscall
        if(fstore->registerStatus(rdx) == 1)saveRegister(rdx);//arguments to syscall
        if(fstore->registerStatus(rcx) == 1)saveRegister(rcx);//trashed by syscall
        if(fstore->registerStatus(r11) == 1)saveRegister(r11);//trashed by syscall
        mov(getImmediateVariable(1),rax);//syscall function (write)
        mov(getImmediateVariable(1),rdi);//file descriptor (stdout)
        if(fstore->stackOffset+c->dataType->size > fstore->stackSize)fstore->stackSize = fstore->stackOffset+c->dataType->size;
        mov(StackPointer,rsi);//char buffer
        add(fstore->stackOffset,rsi);
        variable character;
        character.storage = storageType::MEMORY;
        character.reg = StackPointer;
        character.offsetType = storageType::IMMEDIATE;
        character.offset = fstore->stackOffset;
        character.dataType = c->dataType;
        character.name = "blubblubchar";
        mov(c,&character);
        mov(getImmediateVariable(1),rdx);//only printing 1 character
        code->push_back(getIndent()+"syscall");
        restoreRegisters();
        popRegSave();
    }

    constructor void init()
    {
        _system* sys = new _system;
        sys->name = "gnu-linux";
        sys->printChar = &printChar;
        sys->printStr = &printStr;

        systems.push_back(sys);
    }
}
