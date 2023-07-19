/**
 * Created Date: Monday July 10th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 18th 2023 4:48:23 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
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
#define packed __attribute__((packed))
#define PRINT_DEBUG std::cout << __FILE__  << ":" <<std::dec<< __LINE__ << std::endl;
#include <string>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <list>
#include <vector>

typedef char codechar;
enum class TokenType : uint64_t
{
    INVALID,
    OPERATOR,
    MISC,
    SCOPE,
    TERMINATOR,
    PA
};
std::string tokenToString(TokenType type);
struct token_t
{
    TokenType type;
    uint64_t line;
    char* file;
    uint64_t column;
    codechar* text;
};
std::vector<token_t*> tokenizeFile(char* fname);
