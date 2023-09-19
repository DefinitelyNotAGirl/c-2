/**
 * Created Date: Sunday August 13th 2023
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

void defaultFormat_addScope(uint64_t fID, scope* sc);
void defaultFormat_addClass(uint64_t fID, type* t);
void defaultFormat_addFunction(uint64_t fID, function* func);
void defaultFormat_addVariable(uint64_t fID, variable* var);
void defaultFormat_write(uint64_t fID, std::string path);
uint64_t defaultFormat_newFile();
void* defaultFormat_getDATA(uint64_t fID);

class format
{
public:
    std::string name = "errorFormat";
    std::string extension = "errorFormat";
    std::string desc = "errorDescription";

    void(*addClass)(uint64_t,type*) = &defaultFormat_addClass;
    void(*addFunction)(uint64_t,function*) = &defaultFormat_addFunction;
    void(*addVariable)(uint64_t,variable*) = &defaultFormat_addVariable;
    void(*addScope)(uint64_t,scope*) = &defaultFormat_addScope;
    void(*write)(uint64_t,std::string) = &defaultFormat_write;
    uint64_t(*newFile)() = &defaultFormat_newFile;
    void*(*getDATA)(uint64_t) = &defaultFormat_getDATA;
};

void mOUT(uint64_t fID, variable* var);
void mOUT(uint64_t fID, function* func);
void mOUT(uint64_t fID, type* t);
void mOUT(uint64_t fID, scope* sc);

format* getFormat(std::string name);

extern std::vector<format> formats;
extern std::vector<format*> ClassesOutformats;
extern std::vector<format*> FunctionsOutformats;
extern std::vector<format*> VariablesOutformats;
extern std::vector<format*> ScopesOutformats;
