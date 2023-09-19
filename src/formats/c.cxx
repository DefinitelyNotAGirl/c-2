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

#include <miscout.h>
#include <util.h>

#define constructor __attribute__ ((constructor))

static uint64_t fID = 0;
struct formatFile
{
    uint64_t fID;
    //data
    std::string content;
    bool includesStdInt = false;
};
static std::vector<formatFile*> files;

static std::string getExportTypeName(type* t)
{
    if(t == nullptr)
        std::cout << "wtf?" << std::endl;
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

    if(t->members.size() == 0)
    {
        if(!getFile(fID)->includesStdInt)
        {
            content+="#include <stdint>\n";
            getFile(fID)->includesStdInt = true;
        }

        if(t->size == 0)
            content+="typedef void "+t->name+";\n";
        else if(t->size == 1)
            content+="typedef uint8_t "+t->name+";\n";
        else if(t->size == 2)
            content+="typedef uint16_t "+t->name+";\n";
        else if(t->size == 4)
            content+="typedef uint32_t "+t->name+";\n";
        else if(t->size == 8)
            content+="typedef uint64_t "+t->name+";\n";
    }
    else
    {
        uint64_t cop = 0;//current offset position
        uint64_t pn = 0;//padding number
        content+="struct "+t->name+"\n{\n";
        for(variable& m : t->members)
        {
            if(cop < m.offset)
            {
                if(!getFile(fID)->includesStdInt)
                {
                    content+="#include <stdint>\n";
                    getFile(fID)->includesStdInt = true;
                }

                content+="private:\n";
                uint64_t diff = m.offset-cop;
                while(diff != 0)
                {
                    if(diff >= 8){
                        content+="    uint64_t __cpe2Padding_"+std::to_string(pn++)+";\n";
                        diff-=8;}
                    else if(diff >= 4){
                        content+="    uint32_t __cpe2Padding_"+std::to_string(pn++)+";\n";
                        diff-=4;}
                    else if(diff >= 2){
                        content+="    uint16_t __cpe2Padding_"+std::to_string(pn++)+";\n";
                        diff-=2;}
                    else if(diff >= 1){
                        content+="    uint8_t __cpe2Padding_"+std::to_string(pn++)+";\n";
                        diff--;}
                }
            }
            content+="    "+getExportTypeName(m.dataType)+" "+m.name+";\n";
            cop = m.offset + m.dataType->size;
        }
        content+="};\ntypedef struct "+t->name+" "+t->name+";\n";
    }
}

static void format_addFunction(uint64_t fID, function* func)
{
    if(!func->doExport)
        return;
    std::string& content = getFile(fID)->content;
    content+="extern ";
    content+=getExportTypeName(func->returnType)+" "+func->symbol+"(";
    for(variable* p : func->vparams)
        content+=getExportTypeName(p->dataType)+" "+p->name+",";
    if(func->vparams.size() > 0)
        content.pop_back();
    content+=");\n";
    content+="inline ";
    content+=getExportTypeName(func->returnType)+" "+func->name+"(";
    for(variable* p : func->vparams)
        content+=getExportTypeName(p->dataType)+" "+p->name+",";
    if(func->vparams.size() > 0)
        content.pop_back();
    content+="){"+func->symbol+"(";
    for(variable* p : func->vparams)
        content+=p->name+",";
    if(func->vparams.size() > 0)
        content.pop_back();
    content+=");}\n";
}

static void format_addVariable(uint64_t fID, variable* var)
{
    if(!var->doExport)
        return;
    std::string& content = getFile(fID)->content;
    content+=getExportTypeName(var->dataType)+" "+var->name+";\n";
}

static void format_write(uint64_t fID, std::string path)
{
    fileOut(getFile(fID)->content,path);
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
    f.name = "c";
    f.extension = "h";
    f.desc = "c header files";
    f.addClass = &format_addClass;
    f.addFunction = &format_addFunction;
    f.addVariable = &format_addVariable;
    f.newFile = &format_addFile;
    f.write = &format_write;

    formats.push_back(f);
}
