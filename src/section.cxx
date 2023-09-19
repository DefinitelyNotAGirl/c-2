/*
 * Created Date: Sunday August 20th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday August 20th 2023 2:44:11 pm
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

#include <section.h>

section* DeclSec = new section("decl","");
//section* CtorSec = new section("constructor","ctor");
//section* DtorSec = new section("destructor","dtor");

static section* gSecData = new section("(data) global","data");
static section* gSecRoData = new section("(rodata) global","rodata");
static section* gSecBss = new section("(bss) global","bss");
static section* gSecText = new section("(code) global","text");

void section::push_back(std::string& line)
{
    this->lines.push_back(line);
}

void section::push_back(std::string line)
{
    this->lines.push_back(line);
}
void section::operator<<(std::string line)
{
    this->lines.push_back(line);
}
void section::operator<<(std::string& line)
{
    this->lines.push_back(line);
}

section::section(std::string name, std::string id)
    :name(name),id(id){}

section* getRoDataSec(variable* var)
{
    return new section(".rodata."+var->symbol,"rodata");
}
section* getRoDataSec(function* func)
{
    return new section(".rodata."+func->symbol,"rodata");
}
section* getRoDataSec(type* t)
{
    return new section(".rodata."+t->mangledName,"rodata");
}

section* getDataSec(variable* var)
{
    return new section(".data."+var->symbol,"data");
}
section* getDataSec(function* func)
{
    return new section(".data."+func->symbol,"data");
}
section* getDataSec(type* t)
{
    return new section(".data."+t->mangledName,"data");
}

section* getBssSec(variable* var)
{
    return new section(".bss."+var->symbol,"bss");
}
section* getBssSec(function* func)
{
    return new section(".bss."+func->symbol,"bss");
}
section* getBssSec(type* t)
{
    return new section(".bss."+t->mangledName,"bss");
}

section* getCodeSec(function* func)
{
    return new section(".text."+func->symbol,"text");
}
