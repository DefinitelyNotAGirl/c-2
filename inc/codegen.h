/**
 * Created Date: Wednesday August 2nd 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday August 2nd 2023 4:48:09 am
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
#pragma once

#include <compiler.h>

#define SYNTAX_INTEL 1
#define SYNTAX_GAS 2
#define SYNTAX_NASM 3
#define SYNTAX_INVALID 0
extern uint64_t syntax;

extern std::vector<std::string>* code;
extern functionStorage* fstore;
extern function* codeGenFunc;

variable* getImmediateVariable(uint64_t v);

void pushRegSave();
void popRegSave();
void saveRegister(__register__ reg);
void restoreRegisters();

class location
{
public:
    location(__register__ base, uint64_t offset);
    location(uint64_t offset);
    location(variable* v);
    __register__ base = __register__::invalid;
    uint64_t offset = 0;
    variable* v = nullptr;

    std::string expr();
};

extern arch* currentArch;
extern int ANB;//assembly output number base
void setANB(int n);
void popANB();

extern uint64_t ARCH_REGISTER_MASK;

/*
void genFunctionEpilogue(std::vector<std::string>& lines, scope* sc);
void genFunctionPrologue(std::vector<std::string>& lines, scope* sc);
*/

void codeGenUpdateFuction();

variable* call(function* func,std::vector<variable*>& args);
inline variable* primitiveCall(function* func,std::vector<variable*> args){return currentArch->primitiveCall(func,args);}
void placeSymbol(function* func,std::string symbol);
void placeSymbol(std::string symbol);
void jump(function* func, std::string symbol);
void jump(std::string symbol);
inline void CodePlaceSymbol(std::string symbol){currentArch->CodePlaceSymbol(symbol);}
inline void jmp(std::string symbol){currentArch->jmp(symbol);}
inline void putComment(std::string comment){currentArch->putComment(comment);}

inline void mov(__register__ src,__register__ dst)  {currentArch->movRR(src,dst);}
inline void mov(uint64_t src,__register__ dst)      {currentArch->movUR(src,dst);}
inline void mov(uint64_t src,location dst)          {currentArch->movUL(src,dst);}
inline void mov(location src, location dst)         {currentArch->movLL(src,dst);}
inline void mov(location src, __register__ dst)     {currentArch->movLR(src,dst);}
inline void mov(__register__ src, location dst)     {currentArch->movRL(src,dst);}
inline void mov(variable* src, __register__ dst)    {currentArch->movVR(src,dst);}
inline void mov(__register__ src, variable* dst)    {currentArch->movRV(src,dst);}
inline void mov(variable* src, variable* dst)       {currentArch->movVV(src,dst);}
inline void mov(location src,variable* dst)         {currentArch->movLV(src,dst);}
inline void mov(variable* src,location dst)         {currentArch->movVL(src,dst);}

inline void inc(variable* target){currentArch->inc(target);}
inline void dec(variable* target){currentArch->dec(target);}

inline void add(__register__ src, __register__ dst)     {currentArch->addRR(src,dst);}
inline void add(uint64_t src, __register__ dst)         {currentArch->addUR(src,dst);}
inline void add(variable* dst, variable* a, variable* b){currentArch->addVVV(dst,a,b);}

inline void mul(variable* dst, variable* right, variable* left){currentArch->mulVVV(dst,right,left);}
inline void div(variable* dst, variable* right, variable* left){currentArch->divVVV(dst,right,left);}
inline void mod(variable* dst, variable* right, variable* left){currentArch->modVVV(dst,right,left);}

inline void sub(__register__ src, __register__ dst)     {currentArch->subRR(src,dst);}
inline void sub(uint64_t src, __register__ dst)         {currentArch->subUR(src,dst);}
inline void sub(variable* dst, variable* a, variable* b){currentArch->subVVV(dst,a,b);}

inline void cmp(variable* a, variable* b){currentArch->cmp(a,b);}

inline void jnz(std::string symbol){currentArch->jnz(symbol);}
inline void jne(std::string symbol){currentArch->jne(symbol);}
inline void jno(std::string symbol){currentArch->jno(symbol);}
inline void jns(std::string symbol){currentArch->jns(symbol);}
inline void js(std::string symbol){currentArch->js(symbol);}
inline void jz(std::string symbol){currentArch->jz(symbol);}
inline void je(std::string symbol){currentArch->je(symbol);}
inline void jo(std::string symbol){currentArch->jo(symbol);}
inline void jb(std::string symbol){currentArch->jb(symbol);}
inline void jnae(std::string symbol){currentArch->jnae(symbol);}
inline void jc(std::string symbol){currentArch->jc(symbol);}
inline void jnb(std::string symbol){currentArch->jnb(symbol);}
inline void jae(std::string symbol){currentArch->jae(symbol);}
inline void jnc(std::string symbol){currentArch->jnc(symbol);}
inline void jbe(std::string symbol){currentArch->jbe(symbol);}
inline void jna(std::string symbol){currentArch->jna(symbol);}
inline void jnbe(std::string symbol){currentArch->jnbe(symbol);}
inline void ja(std::string symbol){currentArch->ja(symbol);}
inline void jnge(std::string symbol){currentArch->jnge(symbol);}
inline void jl(std::string symbol){currentArch->jl(symbol);}
inline void jge(std::string symbol){currentArch->jge(symbol);}
inline void jnl(std::string symbol){currentArch->jnl(symbol);}
inline void jle(std::string symbol){currentArch->jle(symbol);}
inline void jng(std::string symbol){currentArch->jng(symbol);}
inline void jnle(std::string symbol){currentArch->jnle(symbol);}
inline void jg(std::string symbol){currentArch->jg(symbol);}
inline void jp(std::string symbol){currentArch->jp(symbol);}
inline void jpe(std::string symbol){currentArch->jpe(symbol);}
inline void jnp(std::string symbol){currentArch->jnp(symbol);}
inline void jpo(std::string symbol){currentArch->jpo(symbol);}
inline void jcxz(std::string symbol){currentArch->jcxz(symbol);}
inline void jecxz(std::string symbol){currentArch->jecxz(symbol);}
