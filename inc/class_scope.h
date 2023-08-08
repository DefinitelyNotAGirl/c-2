/**
 * Created Date: Tuesday August 1st 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday August 1st 2023 9:27:40 pm
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

enum class scopeType : uint64_t
{
    INVALID,GLOBAL,FUNCTION,CLASS
};

class scope
{
public:
    std::string name;
    bool isIndentBased = false;
    uint64_t leadingSpace = 0;
    std::vector<variable*> variables;
    scopeType t = scopeType::GLOBAL;
    std::vector<function*> functions;
    scope* parent = nullptr;
    function* func = nullptr;
    functionStorage* fstore = nullptr;//this should only be filled if t==scopeType::FUNCTION
};