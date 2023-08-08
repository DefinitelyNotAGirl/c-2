/**
 * Created Date: Sunday July 30th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday July 30th 2023 4:53:27 am
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

#include <common.h>
#include <compiler.h>

namespace error
{
    void compilerBug(std::string file, int line);
    void noSuchType(token& t);
    void noSuchLitop(token& t, uint64_t offset = 0);
    void expectedNewUnique(token& t);
    void expectedShortop(token& t);
    void invalidFunctionAttribute(token& t);
    void invalidVariableAttribute(token& t);
    void expectedTypename(token& t);
}

#define errorCompilerBug error::compilerBug(__FILE__,__LINE__)
