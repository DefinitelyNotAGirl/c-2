/**
 * Created Date: Wednesday July 19th 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday July 19th 2023 6:13:35 pm
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
#include <compiler.hxx>

namespace error
{
    void variableNameLatinChar(uint64_t tPOS, std::vector<token_t*>& tokens);
    void terminatorB4Token(uint64_t tPOS, std::vector<token_t*>& tokens);
    void invalidLitopOP(uint64_t tPOS, std::vector<token_t*>& tokens);
    void litopValueImmediate(uint64_t tPOS, std::vector<token_t*>& tokens);
    void noSuchType(uint64_t tPOS, std::vector<token_t*>& tokens);
}