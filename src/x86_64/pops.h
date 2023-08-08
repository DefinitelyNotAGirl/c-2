/**
 * Created Date: Thursday August 3rd 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 3rd 2023 1:55:22 pm
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

#include "master.h"

class location
{
public:
    location(__register__ base, uint64_t offset);
    location(uint64_t offset);
    __register__ base;
    uint64_t offset;

    std::string expr();
};

void mov(__register__ src,__register__ dst);
void mov(uint64_t src,__register__ dst);
void mov(uint64_t src,location dst);
void mov(location src, location dst);
void mov(location src, __register__ dst);
void mov(__register__ src, location dst);
void mov(variable* src, __register__ dst);
void mov(__register__ src, variable* dst);
void mov(variable* src, variable* dst);
void mov(variable* src, __register__ dst);
void mov(location src,variable* dst);
void mov(variable* src,location dst);

void add(__register__ src, __register__ dst);
void add(uint64_t src, __register__ dst);

void mul(variable* dst, variable* right, variable* left);

void mod(variable* dst, variable* right, variable* left);

void sub(__register__ src, __register__ dst);
void sub(uint64_t src, __register__ dst);
