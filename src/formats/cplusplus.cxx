/*
 * Created Date: Sunday August 13th 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 17th 2023 8:36:32 pm
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

#include <miscout.h>

#define constructor __attribute__ ((constructor))

static uint64_t fID = 0;
struct formatFile
{
    uint64_t fID;
    //data
    std::string content;
};
static std::vector<formatFile*> files;

static std::string getExportTypeName(type* t)
{
    if(t->members.size() == 0)
    {
        if(t->size == 0)
    }
    return t->name;
}

static formatFile* getFile(uint64_t fID)
{
    for(formatFile* ff : files)
        if(ff->fID == fID)
            return ff;
    return nullptr;
}

static void format_addClass(uint64_t fID, type* t)
{
    if(!t->doExport)
        return;
    std::string& content = getFile(fID)->content;
}

static void format_addFunction(uint64_t fID, function* func)
{
    if(!func->doExport)
        return;
    std::string& content = getFile(fID)->content;
}

static void format_addVariable(uint64_t fID, variable* var)
{
    if(!var->doExport)
        return;
    std::string& content = getFile(fID)->content;
    content+=getExportTypeName(var->dataType)+" "+var->name+";\n";
}

static uint64_t format_addFile()
{
    fID++;
    files.push_back(new formatFile);
    files.back()->fID = fID;

    return fID;
}

//this function must be static in order to not cause duplicate symbol issues with other formats
//"constructor" tells the compiler to execute this before main, 
// which means we can just add more of these functions and the compiler will do the heavy lifting
constructor static void format_init()
{
    format f;
    f.name = "c++";
    f.extension = "hpp";
    f.addClass = &format_addClass;
    f.addFunction = &format_addFunction;
    f.addVariable = &format_addVariable;
    f.newFile = &format_addFile;

    formats.push_back(f);
}
