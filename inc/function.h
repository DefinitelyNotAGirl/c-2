/**
 * Created Date: Wednesday August 2nd 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 17th 2023 9:04:51 pm
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

enum class primitiveOP : uint64_t
{
    invalid = 0,
    add,
    sub,
    mul,
    div,
    mod,
    assign,
    _or,
    _xor,
    _and,
    equal,
    NotEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    Not,
    Inc,
    Dec,
    Index,
    Interrupt,
    CPUid,
    PRINTCHAR,
    PRINTSTR,
    SYSCALL
};

/*
    miscData:
        0: is cast function
        1-63: reserved MBZ
*/
class function
{
public:
    std::string name;
    std::string symbol;

    std::string desc;
    std::string returnDesc;

    type* returnType;
    uint64_t miscData = 0;
    std::vector<type*> parameters;
    std::vector<variable*> vparams;
    std::vector<std::string> code;
    ABI* abi = nullptr;
    functionStorage* fstore;
    std::string __declared_file;
    uint64_t __declared_line;

    bool isPrimitive = false;
    primitiveOP op = primitiveOP::invalid;
    bool primitiveInPlace = false;
    bool primitiveFloat = false;
    bool isDeprecated = false;
    bool noReturn = false;
    bool noDoc = false;
    bool isLocal = false;//static
    bool doExport = false;

    bool ignoreCall = false;
};

struct castFunction
{
    type* input;
    type* output;
    function* func;
};