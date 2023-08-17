/*
 * Created Date: Sunday August 13th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday August 13th 2023 3:22:01 pm
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
};
static std::vector<formatFile*> files;

static formatFile* getFile(uint64_t fID)
{
    for(formatFile* ff : files)
        if(ff->fID == fID)
            return ff;
    return nullptr;
}

static void format_addClass(uint64_t fID, type* t)
{
}

static void format_addFunction(uint64_t fID, function* func)
{
    std::cout << "##############" << std::endl;
    std::cout << "function: " << func->name << std::endl;
    std::cout << "symbol: " << func->symbol << std::endl;
    std::cout << "ABI: ";
    switch(func->fstore->ABI)
    {
        case(1):
            std::cout << "SystemV amd64" << std::endl;
            break;
        case(2):
            std::cout << "Microsoft x64" << std::endl;
            break;
        default:
            std::cout << "Unknown" << std::endl;
    }
    std::cout << "return type: " << func->returnType->name << std::endl;
    if(func->parameters.size() > 0)
    {
        std::cout << "parameters: " << std::endl;
        for(type* p : func->parameters)
            std::cout << "        " << p->name << std::endl;
    }
    std::cout << std::endl;
}

static void format_addVariable(uint64_t fID, variable* var)
{
    std::cout << "##############" << std::endl;
    std::cout << "variable: " << var->name << std::endl;
    std::cout << "symbol: " << var->symbol << std::endl;
    std::cout << "type: " << var->dataType->name << std::endl;
    std::cout << "storage: ";
    if(currentScope->t == scopeType::FUNCTION)
    {
        if(var->storage == storageType::MEMORY)
        {
            if(var->offset < 0)
                std::cout << "SP";
            else
                std::cout << "SP+";
            std::cout <<std::dec<< (int64_t)var->offset;
        }
        else if(var->storage == storageType::REGISTER)
        {
            std::cout << registerNAME(var->reg);
        }
        else if(var->storage == storageType::MEMORY_ABSOLUTE)
        {
            std::cout << "0x" << std::hex << var->offset;
        }
    }
    else
    {
        if(var->storage == storageType::REGISTER)
        {
            std::cout << registerNAME(var->reg);
        }
        else if(var->storage == storageType::MEMORY_ABSOLUTE)
        {
            std::cout << "0x" << std::hex << var->offset;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

static void format_addScope(uint64_t fID, scope* sc)
{
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
    f.name = "stdout";
    f.extension = "stdout";
    f.addClass = &format_addClass;
    f.addFunction = &format_addFunction;
    f.addScope = &format_addScope;
    f.addVariable = &format_addVariable;
    f.newFile = &format_addFile;

    formats.push_back(f);
}
