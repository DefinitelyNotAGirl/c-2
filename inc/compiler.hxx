/*
 * Created Date: Monday July 10th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday July 10th 2023 6:04:24 pm
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

#include <common.h>
#include <vector>

std::list<std::string> dependencies;

enum class exprType : uint32_t {
    IMM,CALL,VAR
};

struct expression
{
    exprType type;
} packed;

struct exprIMM
{
    exprType type;
    uint64_t vType;
    uint64_t value;
} packed;

struct exprCall
{
    exprType type;
    uint64_t func;
    expression** args;
} packed;

struct type;

struct function
{
    codechar* name;
    type* dType;
} packed;

struct variable
{
    codechar* name;
    variable* args;
    type* returnType;
} packed;

struct type
{
    codechar* name;
    function* functions;
    variable* members;
} packed;

std::vector<type> types;
std::vector<function> functions;
std::vector<variable> variables;

expression* resolveExpression(std::string& token);
