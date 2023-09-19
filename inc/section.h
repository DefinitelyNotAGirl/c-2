/**
 * Created Date: Sunday August 20th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday August 20th 2023 2:34:51 pm
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

class section
{
public:
    std::string name;
    std::list<std::string> lines;
    std::string id;

    section(std::string name, std::string id);

    void push_back(std::string line);
    void push_back(std::string& line);

    void operator<<(std::string line);
    void operator<<(std::string& line);
};

extern section* DeclSec;
//extern section* CtorSec;
//extern section* DtorSec;

section* getRoDataSec(variable* var);
section* getRoDataSec(function* func);
section* getRoDataSec(type* t);

section* getDataSec(variable* var);
section* getDataSec(function* func);
section* getDataSec(type* t);

section* getBssSec(variable* var);
section* getBssSec(function* func);
section* getBssSec(type* t);

section* getCodeSec(function* func);
