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
#include <sys/resource.h>
#include <codegen.h>
#include <DWARF.h>

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

std::string __reqFileVSTC = "";

int main(int argc, char** argv)
{
    signal(SIGSEGV, HANDLER_SIGSEGV);   // install our handler
    signal(SIGABRT, HANDLER_SIGSEGV);   // install our handler
    signal(SIGILL, HANDLER_SIGSEGV);   // install our handler
    //resize stack
    const rlim_t kStackSize = 16 * 1024 * 1024;// min stack size = 16MiB
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        //std::cout << "current stack limit: 0x" << std::hex << rl.rlim_cur << std::endl;
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
            else
            {
                //std::cout << "stack limit set to: 0x" << std::hex << kStackSize << std::endl;
            }
        }
    }
    //get working directory
    char* workingDir = getcwd(nullptr,0);//only works for GNU libc, must find alternative soloution for other systems
    std::string cwd = workingDir;
    //initiate compiler
    initWarnings();
    setDefaults();
    cliOptions(argc, argv);
    if(options::ddebug)
    {
        std::cout << "#\n#\n#\n#" << "c+=2 compiler running" << "\n#\n#\n#" << std::endl;
    }
    if(options::aso)
        options::as = true;
    if(options::fcpl != 3)
        std::cout << "cpu privilege level: " << options::fcpl << std::endl;
    globalScope->name = "global";
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

    //set stdlib include dir
    if(!(options::ffreestanding || options::fnostdlib))
        includeDirs.push_back("/usr/local/include/cpe2/");

    for(std::string i : sourceFiles)
    {
        if(!options::fnoautoinclude)
        {
            std::vector<line> stdLines;
            stdLines.push_back(defLine("#include <stdint>"));
            stdLines.push_back(defLine("#include <userspace>"));
            stdLines.push_back(defLine("#include <lang>"));
            stdLines.push_back(defLine("#include <Memory>"));
            stdLines.push_back(defLine("#include <OperatingSystem>"));
            if(!options::fnolibc)
                stdLines.push_back(defLine("#include <libc>"));
            parse(stdLines);
        }
        if(!options::C)
        {
            if(!options::ffreestanding)
            {
                startObjFiles.push_back("/usr/local/lib/cpe2/crt0.o");
                startObjFiles.push_back("/usr/local/lib/cpe2/crtsig.o");
                startObjFiles.push_back("/usr/local/lib/cpe2/exit.o");
            }
        }

        std::vector<line> lines = getLines(i);
        currentFile = i;
        if(options::vstc || options::vsls)
            __reqFileVSTC = currentFile;
        parse(lines);
        //finish up debug information
        if(true /*check for GAS (true for now)*/ && options::debugSymbols)
        {
            std::vector<std::string> dbgCode = DebugCode;
            std::vector<std::string> dbgAbCode = DebugAbbrevCode;
            DebugCode.clear();
            DebugAbbrevCode.clear();
            code = &DebugCode;
            setANB(16);
            putComment("unit header");
            DebugCode.push_back("__debug_info_start:");
            DebugCode.push_back("\t.int 0xffffffff");//4-byte 0xffffffff as mandated by DWARF-5
            DebugCode.push_back("\t.quad __debug_info_end - (__debug_info_start+12)");//debug_info size
            DebugCode.push_back("\t.word 5");//DWARF version 5
            DebugCode.push_back("\t.byte "+intToString((uint64_t)DWARF5::UT_compile));//DW_UT_compile
            DebugCode.push_back("\t.byte "+intToString(POINTER_SIZE));//address size in bytes
            DebugCode.push_back("\t.quad debugAbbrev");//offset into debug_abbrev section
            //dwaft compilation unit
            DebugCode.push_back("\t.uleb128 "+intToString(1));
            DebugAbbrevCode.push_back("\t.uleb128 "+intToString(1));
            DebugAbbrevCode.push_back("\t.uleb128 "+intToString((uint64_t)DWARF5::TAG_compile_unit));
            DebugAbbrevCode.push_back("\t.byte 1");//bool indicating the presence of child tags (0 for testing purposes)
            DebugAbbrevCode.push_back("\t.uleb128 "+intToString((uint64_t)DWARF5::AT_name));
            DebugAbbrevCode.push_back("\t.uleb128 "+intToString((uint64_t)DWARF5::FORM_string));
            DebugCode.push_back("\t.string \""+i+"\"");
            DebugAbbrevCode.push_back("\t.uleb128 "+intToString((uint64_t)DWARF5::AT_comp_dir));
            DebugAbbrevCode.push_back("\t.uleb128 "+intToString((uint64_t)DWARF5::FORM_string));
            DebugCode.push_back("\t.string \""+cwd+"\"");
            DebugAbbrevCode.push_back("\t.uleb128 0");//null, terminate
            DebugAbbrevCode.push_back("\t.uleb128 0");
            //re-add debug code
            for(std::string& i : dbgCode)
                DebugCode.push_back(i);
            for(std::string& i : dbgAbCode)
                DebugAbbrevCode.push_back(i);
            DebugCode.push_back("__debug_info_end:");
        }
        //output
        std::string rname = i;
        FILE* f;
        stripExt(rname);
        rname = rname.substr(rname.find_first_of('/'),rname.length());
        //generate depedency make files
        //get output destiations
        std::filesystem::create_directories(options::buildDir);
        if(options::buildDir != "")
        {
            objOut = options::buildDir+rname+".o";
            asmOut = options::buildDir+rname+".a86";//must be changed for intel syntax
            mdOut = options::buildDir+rname+".d";
            execOut = options::buildDir+rname+".exe";
            //use .exe on all platforms for now, should not cause issues (queue linux trying to load as PE executable)
        }
        else if(options::output == "")
        {
            //no output destination specified
            objOut = rname+".o";
            asmOut = rname+".a86";//must be changed for intel syntax
            mdOut = rname+".d";
            execOut = rname+".exe";
            //use .exe on all platforms for now, should not cause issues (queue linux trying to load as PE executable)
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