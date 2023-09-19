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
#include "gdoc.h"

#define constructor __attribute__ ((constructor))

namespace formatGDOC
{

static uint64_t fID = 0;

static std::vector<geosdoc::formatFile*> files;

static geosdoc::formatFile* getFile(uint64_t fID)
{
    for(geosdoc::formatFile* ff : files)
        if(ff->fID == fID)
            return ff;
    return nullptr;
}

static uint64_t format_addFile()
{
    fID++;
    files.push_back(new geosdoc::formatFile);
    files.back()->fID = fID;

    return fID;
}

static void* format_getDATA(uint64_t fID)
{
    return getFile(fID);
}

static void format_addClass(uint64_t fID, type* t)
{
    geosdoc::formatFile* f = getFile(fID);
    if(f == nullptr)
        return;
    if(!t->nodoc)
        if(!contains<type*>(f->classes,t))
            f->classes.push_back(t);
}

static void format_addFunction(uint64_t fID, function* func)
{
    geosdoc::formatFile* f = getFile(fID);
    if(f == nullptr)
        return;
    if(!func->noDoc)
        if(!contains<function*>(f->functions,func))
            f->functions.push_back(func);
}

static void format_addVariable(uint64_t fID, variable* var)
{
    geosdoc::formatFile* f = getFile(fID);
    if(f == nullptr)
        return;
    if(currentScope->t == scopeType::CLASS || currentScope->t == scopeType::FUNCTION)
        return;

    if(!contains<variable*>(f->variables,var))
        f->variables.push_back(var);
}

static void format_write(uint64_t fID, std::string path)
{
    std::string out = "";
    geosdoc::formatFile* ff = getFile(fID);
    if(ff->lastPath != "")
        return;

    out += "#title untitled\n";
    for(type* t : ff->classes)
    {
        out += "<class>\n";
        out += "    <name>"+t->name+"</name>\n";
        for(variable& var : t->members)
        {
            out += "    <member>\n";
            out += "        <name>"+var.name+"</name>\n";
            out += "        <type><name>"+var.dataType->name+"</name></type>\n";
            out += "    </member>\n";
        }
        out += "</class>\n";
    }

    for(function* func : ff->functions)
    {
        out += "<function>\n";
        out += "    <name>"+func->name+"</name>\n";
        out += "    <attributes>";
        if(func->isDeprecated)
            out+="deprecated ";
        out += "    </attributes>\n";
        out+="    <type><name>"+func->returnType->name+"</name></type>\n";
        for(variable* p : func->vparams)
        {
            out+="    <param>\n";
            out+="        <name>"+p->name+"</name>\n";
            out+="        <type><name>"+p->dataType->name+"</name></type>\n";
            out+="    </param>\n";
        }
        out += "</function>\n";
    }

    for(variable* var : ff->variables)
    {
        out += "<member>\n";
        out += "    <name>"+var->name+"</name>\n";
        out += "    <type><name>"+var->dataType->name+"</name></type>\n";
        out += "</member>\n";
    }

    stripExt(path);
    path+=".gdoc";
    fileOut(out,path);

    system(std::string("gdoc "+path+" -o "+path+".gdml").c_str());
    ff->lastPath=path+".gdml";
}

//this function must be static in order to not cause duplicate symbol issues with other formats
//"constructor" tells the compiler to execute this before main, 
// which means we can just add more of these functions and the compiler will do the heavy lifting
constructor static void format_init()
{
    format f;
    f.name = "geosdoc";
    f.extension = "gdoc";
    f.desc = "geosdoc code for further proccessing";
    f.addClass = &format_addClass;
    f.addFunction = &format_addFunction;
    f.addVariable = &format_addVariable;
    f.write = &format_write;
    f.newFile = &format_addFile;
    f.getDATA = &format_getDATA;

    formats.push_back(f);
}

}
