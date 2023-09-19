/**
 * Created Date: Sunday September 17th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday September 17th 2023 4:54:45 am
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

class location;

class arch
{
public:
    std::string name;

    //variable* (*call)(function* func,std::vector<variable*> args);
    variable* (*primitiveCall)(function* func,std::vector<variable*> args);
    void (*CodePlaceSymbol)(std::string symbol);
    void (*putComment)(std::string comment);
    void(*activate)();

    void(*cmp)(variable* a, variable* b);
    void(*jmp)(std::string symbol);
    void(*jnz)(std::string symbol);
    void(*jne)(std::string symbol);
    void(*jno)(std::string symbol);
    void(*jns)(std::string symbol);
    void(*js)(std::string symbol);
    void(*jz)(std::string symbol);
    void(*je)(std::string symbol);
    void(*jo)(std::string symbol);
    void(*jb)(std::string symbol);
    void(*jnae)(std::string symbol);
    void(*jc)(std::string symbol);
    void(*jnb)(std::string symbol);
    void(*jae)(std::string symbol);
    void(*jnc)(std::string symbol);
    void(*jbe)(std::string symbol);
    void(*jna)(std::string symbol);
    void(*jnbe)(std::string symbol);
    void(*ja)(std::string symbol);
    void(*jnge)(std::string symbol);
    void(*jl)(std::string symbol);
    void(*jge)(std::string symbol);
    void(*jnl)(std::string symbol);
    void(*jle)(std::string symbol);
    void(*jng)(std::string symbol);
    void(*jnle)(std::string symbol);
    void(*jg)(std::string symbol);
    void(*jp)(std::string symbol);
    void(*jpe)(std::string symbol);
    void(*jnp)(std::string symbol);
    void(*jpo)(std::string symbol);
    void(*jcxz)(std::string symbol);
    void(*jecxz)(std::string symbol);

    void(*movRR)(__register__ src,__register__ dst);
    void(*movUR)(uint64_t src,__register__ dst);
    void(*movUL)(uint64_t src,location dst);
    void(*movLL)(location src, location dst);
    void(*movLR)(location src, __register__ dst);
    void(*movRL)(__register__ src, location dst);
    void(*movVR)(variable* src, __register__ dst);
    void(*movRV)(__register__ src, variable* dst);
    void(*movVV)(variable* src, variable* dst);
    void(*movLV)(location src,variable* dst);
    void(*movVL)(variable* src,location dst);

    void(*inc)(variable* target);
    void(*dec)(variable* target);

    void(*addRR)(__register__ src, __register__ dst);
    void(*addUR)(uint64_t src, __register__ dst);
    void(*addVVV)(variable* dst, variable* a, variable* b);

    void(*mulVVV)(variable* dst, variable* right, variable* left);
    void(*divVVV)(variable* dst, variable* right, variable* left);
    void(*modVVV)(variable* dst, variable* right, variable* left);

    void(*subRR)(__register__ src, __register__ dst);
    void(*subUR)(uint64_t src, __register__ dst);
    void(*subVVV)(variable* dst, variable* a, variable* b);
};
extern std::vector<arch*> architectures;
arch* getArch(std::string name);
