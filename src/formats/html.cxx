/*
 * Created Date: Tuesday August 15th 2023
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
#include "gdoc.h"
#include <unordered_set>

#define constructor __attribute__ ((constructor))

static uint64_t fID = 0;
struct formatFile
{
    uint64_t fID;
    //data
};
static std::vector<geosdoc::formatFile*> files;

static geosdoc::formatFile* getFile(uint64_t fID)
{
    for(geosdoc::formatFile* ff : files)
        if(ff->fID == fID)
            return ff;
    return nullptr;
}

static format* gdoc = nullptr;
static void format_addClass(uint64_t fID, type* t){gdoc->addClass(fID,t);}
static void format_addFunction(uint64_t fID, function* t){gdoc->addFunction(fID,t);}
static void format_addVariable(uint64_t fID, variable* t){gdoc->addVariable(fID,t);}
static void format_addScope(uint64_t fID, scope* t){gdoc->addScope(fID,t);}
static void format_write(uint64_t fID, std::string path)
{
    gdoc->write(fID,path);
    geosdoc::formatFile* ff = gdoc->getDATA(fID);
    std::string outpath = ff->lastPath;
    system(std::string("gdml_html "+ff->lastPath+" -o "+path).c_str());
}
static uint64_t format_addFile()
{
    fID++;
    files.push_back(new geosdoc::formatFile);
    files.back()->fID = fID;

    if(gdoc == nullptr)
    {
        gdoc = getFormat("geosdoc");
        if(gdoc == nullptr)
        {
            std::cout << "ERROR: this html generator requires the \"geosdoc\" format in order to work!";
            format* f = getFormat("html");
            *f = format();
            f->name = "html";
            f->extension = "html";
        }
        else
        {
            std::unordered_set<format*> oFormats;
            bool hasGeosDoc = false;
            for(format* f : ClassesOutformats)
                oFormats.insert(f);
            for(format* f : FunctionsOutformats)
                oFormats.insert(f);
            for(format* f : VariablesOutformats)
                oFormats.insert(f);
            for(format* f : ScopesOutformats)
                oFormats.insert(f);
            for(format* f : oFormats)
                if(f->name == "geosdoc")
                    hasGeosDoc = true;
            if(!hasGeosDoc)
                gdoc->newFile();
        }
    }

    return fID;
}

//this function must be static in order to not cause duplicate symbol issues with other formats
//"constructor" tells the compiler to execute this before main, 
// which means we can just add more of these functions and the compiler will do the heavy lifting
constructor static void format_init()
{
    format f;
    f.name = "html";
    f.extension = "html";
    f.desc = "html interactive web documentation using the gdoc format";
    f.addClass = &format_addClass;
    f.addFunction = &format_addFunction;
    f.addScope = &format_addScope;
    f.addVariable = &format_addVariable;
    f.write = &format_write;
    f.newFile = &format_addFile;

    formats.push_back(f);
}
