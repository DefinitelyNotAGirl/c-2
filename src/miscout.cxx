/*
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

#include <compiler.h>
#include <miscout.h>

void defaultFormat_addScope(uint64_t fID, scope* sc){}
void defaultFormat_addClass(uint64_t fID, type* t){}
void defaultFormat_addFunction(uint64_t fID, function* func){}
void defaultFormat_addVariable(uint64_t fID, variable* var){}
void defaultFormat_write(uint64_t fID, std::string path){}
uint64_t defaultFormat_newFile(){}
void* defaultFormat_getDATA(uint64_t fID){return nullptr;}

format* getFormat(std::string name)
{
    for(format& i : formats)
        if(i.name == name)
            return &i;
    return nullptr;
}

void mOUT(uint64_t fID, variable* var)
{
    for(format* i : VariablesOutformats)
        i->addVariable(fID,var);
}

void mOUT(uint64_t fID, function* func)
{
    for(format* i : FunctionsOutformats)
    {
        i->addFunction(fID,func);
    }
}
void mOUT(uint64_t fID, type* t)
{
    for(format* i : ClassesOutformats)
        i->addClass(fID,t);
}

void mOUT(uint64_t fID, scope* sc)
{
    for(format* i : ScopesOutformats)
        i->addScope(fID,sc);
}
