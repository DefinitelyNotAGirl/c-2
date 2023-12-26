/**
 * Created Date: Wednesday August 30th 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday August 30th 2023 6:27:58 pm
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

class templateArg
{
public:
    //types:
    //0: invalid
    //1: integer
    //2: typename
    //3: float
    uint64_t Type = 0;
    std::string name;
};

class templateInstance
{
public:
    void* ft = nullptr;// ft = ttype ? type* : function*
    bool ttype = false;
    std::string mangled = "";//`template_${function/type.name}_${args seperated by underscores}`
};

class typeTemplate
{
public:
    scope* sc;
    std::string name;
    std::vector<templateInstance*> instances;
    std::vector<templateArg*> tArgs;
    std::vector<line> code;
};

class functionTemplate
{
public:
    scope* sc;
    std::string name;
    std::vector<templateArg*> tArgs;
    std::vector<line> code;
};

extern std::vector<typeTemplate*> typeTemplates;
extern std::vector<functionTemplate*> functionTemplates;
