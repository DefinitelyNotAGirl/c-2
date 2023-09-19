/*
 * Created Date: Tuesday July 18th 2023
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

#include <options.h>
#include <common.h>
#include <compiler.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <util.h>
#include <miscout.h>
#include <unordered_set>
#include <filesystem>
#include <mangling.h>

void cliOptions(int argc, char **argv);
std::vector<line> getLines(std::string fname);
void parse(std::vector<line> lines);
void genOutput(std::string& i);
void initWarnings();
void setDefaults();

void HANDLER_SIGSEGV(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 50);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

line defLine(std::string text)
{
    line l;
    l.file = "default code from compiler";
    l.leadingSpaces = 0;
    l.lineNum = 0;
    l.text = text;
    return l;
}

int main(int argc, char** argv)
{
    signal(SIGSEGV, HANDLER_SIGSEGV);   // install our handler
    signal(SIGABRT, HANDLER_SIGSEGV);   // install our handler
    signal(SIGILL, HANDLER_SIGSEGV);   // install our handler

    initWarnings();

    setDefaults();

    cliOptions(argc, argv);

    if(options::fcpl != 3)
        std::cout << "cpu privilege level: " << options::fcpl << std::endl;

    if(options::ffreestanding)
        options::fnoautoinclude = true;

    moClassID = 1;
    moFunctionID = 1;
    moVariableID = 1;
    moScopeID = 1;
    std::vector<format*> preOFormats;
    std::vector<format*> oFormats;
    for(format* f : ClassesOutformats)
        preOFormats.push_back(f);
    for(format* f : FunctionsOutformats)
        preOFormats.push_back(f);
    for(format* f : VariablesOutformats)
        preOFormats.push_back(f);
    for(format* f : ScopesOutformats)
        preOFormats.push_back(f);
    for(format* f : preOFormats)
        if(!contains<format*>(oFormats,f))
            oFormats.push_back(f);
        //else
        //    std::cout << "duplicate format: " << f->name << std::endl;

    //for(format* f : oFormats)
    //{
    //    std::cout << "format: " << f->name << std::endl;
    //    std::cout << "address: " << std::hex << (void*)f << std::endl;
    //}
    
    for(format* f : oFormats)
        if(f->newFile() != 1)
            std::cout << "Fault format: " << f->name << std::endl;
        //else
        //    std::cout << "initialized format: " << f->name << std::endl;

    for(std::string i : sourceFiles)
    {
        if(!options::fnoautoinclude)
        {
            std::vector<line> stdLines;
            stdLines.push_back(defLine("#include <stdint>"));
            stdLines.push_back(defLine("#include <Memory>"));
            stdLines.push_back(defLine("#include <OperatingSystem>"));
            if(!options::fnolibc)
                stdLines.push_back(defLine("#include <libc>"));
            parse(stdLines);
        }

        std::vector<line> lines = getLines(i);
        parse(lines);
        std::string rname = i;
        FILE* f;
        stripExt(rname);
        rname = rname.substr(rname.find_first_of('/'),rname.length());
        //generate depedency make files
        //get output destiations
        std::filesystem::create_directories(options::buildDir);
        if(options::buildDir != "")
        {
            objOut = options::buildDir+"/"+rname+".o";
            asmOut = options::buildDir+"/"+rname+".s";
        }
        else if(options::output == "")
        {
            //no output destination specified
            objOut = rname+".o";
            asmOut = rname+".s";
        }
        else
        {
            struct stat s;
            if(access(options::output.c_str(), F_OK) != 0)
                goto outputIsFile;
            if(stat(options::output.c_str(),&s) == 0)
            {
                if(s.st_mode & S_IFDIR)
                {
                    // it's a directory
                    objOut = options::output+"/"+rname+".o";
                    asmOut = options::output+"/"+rname+".s";
                }
                else if(s.st_mode & S_IFREG)
                {
                    // it's a file
                    outputIsFile:;
                    objOut = options::output;
                    stripExt(options::output);
                    asmOut = options::output+".s";
                }
                else
                {
                    // something else
                }
            }
            else
            {
                std::cout << "ERROR: could not access output destination \"" << options::output << "\"" << std::endl;
                // error
            }
        }
        genOutput(i);
        std::filesystem::create_directories(options::docDir);
        for(format* f : oFormats)
        {
            std::string oname = options::docDir+"/"+rname+"."+f->extension;
            //std::cout << "outputting: " << oname << std::endl;
            f->write(1,oname);
        }

        //reset compiler
        resetScope();
    }
    return 0;
}