/**
 * Created Date: Wednesday September 13th 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday September 13th 2023 2:34:38 am
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
#include <util.h>

class ABI
{
public:
    std::string name;
    std::vector<__register__> VolatileRegisters;
    std::vector<__register__> nonVolatile;

    void(*setArgStorages)(function* func,std::vector<variable*>& args);
    void(*moveArguments)(function* func,std::vector<variable*>& args);
    void(*genProlouge)(std::vector<std::string>& lines, scope* sc);
    void(*genEpilouge)(std::vector<std::string>& lines, scope* sc);
    void(*preCall)(function* func);
    void(*postCall)(function* func);
    void(*instrCall)(function* func);
    variable*(*call)(function* func,std::vector<variable*>& args);

    __register__ integerReturn = __register__::invalid;
    __register__ floatReturn = __register__::invalid;
};

extern std::vector<ABI*> ABIs;
