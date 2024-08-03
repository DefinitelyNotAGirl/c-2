/*
 * Created Date: Tuesday July 18th 2023
 * Author: Lilith
 * -----
 * Last Modified: Fri Aug 02 2024
 * Modified By: Lilith
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
#include <DWARF.h>
#include <error.h>
#include <EntityManagement.hxx>

#include <Parser.hxx>

#include <stacktrace.hxx>

void cliOptions(int argc, char **argv);
std::vector<line> getLines(std::string fname);
void genOutput(std::string& i);
void initWarnings();
void setDefaults();

line defLine(std::string text)
{
    line l;
    l.file = "default code from compiler";
    l.leadingSpaces = 0;
    l.lineNum = 0;
    l.text = text;
    return l;
}

#include <resources.hxx>

//#define CPE2_BUILD_TEST

std::string __reqFileVSTC = "";
extern std::stack<bool> isTemplateInstance;
void test_main();
void output_init();
void warn_init();
void error_init();
void install_crash_handlers();

int main(int argc, char** argv)
{
	install_crash_handlers();
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
	output_init();
	warn_init();
	error_init();
	#ifdef CPE2_BUILD_TEST
		test_main();
		return 0;
	#endif
    //get working directory
    char* workingDir = getcwd(nullptr,0);//only works for GNU libc, must find alternative soloution for other systems
    std::string cwd = workingDir;
    //initiate compiler
	isTemplateInstance.push(false);
    initWarnings();
    setDefaults();
    cliOptions(argc, argv);
    if(options::ddebug)
    {
        std::cout << "#\n#\n#\n#" << "c+=2 compiler running" << "\n#\n#\n#" << std::endl;
    }
    if(options::aso)
        options::as = true;
    if(options::fcpl != 3 && options::ddebug)
        std::cout << "cpu privilege level: " << options::fcpl << std::endl;
    globalScope->name = "global";
	globalScope->func = new function;
	globalScope->func->code = new section;
	globalScope->func->abi = defaultABI;
	globalScope->func->returnType = nullptr;
	globalScope->func->name = "global function";
	globalScope->func->symbol = "cpe2InitiateGlobals";
	Entity::updateCurrentScope(globalScope);
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
        includeDirs.push_back("../stdcpe2/inc");
	
	//,
	//, set output destinations
	//,
	std::string rname;
	{
		//output
		if(options::output != "")
        	rname = options::output;
		else
			rname = sourceFiles.front();
        FILE* f;
		std::string ext = getExt(rname);
		if(ext != "c2resource")
        	stripExt(rname);
        rname = rname.substr(rname.find_first_of('/'),rname.length());
        //generate depedency make files
        //get output destiations
        if(options::buildDir != "")
            std::filesystem::create_directories(options::buildDir);
        if(options::buildDir != "")
        {
            objOut = options::buildDir+rname+".o";
            mdOut = options::buildDir+rname+".d";
            execOut = options::buildDir+rname+".exe";
			resOut = options::buildDir+rname+".c2resource";
            //use .exe on all platforms for now, should not cause issues (queue linux trying to load as PE executable)
        }
        else if(options::output == "")
        {
            //no output destination specified
            objOut = rname+".o";
            mdOut = rname+".d";
			resOut = rname+".c2resource";
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
					resOut = options::output+"/"+rname+".c2resource";
                }
                else if(s.st_mode & S_IFREG)
                {
                    // it's a file
                    outputIsFile:;
                    objOut = options::output;
                    stripExt(options::output);
                    asmOut = options::output+".s";
                    mdOut = options::output+".d";
					resOut = options::output+".c2resource";
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
	}

    for(std::string i : sourceFiles)
    {
		if(true)
		{
			//declare default types
			std::vector<Entity::Attribute> noAttribs({});
			anyType = Entity::declareType(noAttribs,"____cpe2_type_any",std::vector<token>({token("primitive0")}));
			voidType = Entity::declareType(noAttribs,"void",std::vector<token>({token("primitive0")}));
			defaultUnsignedIntegerType = Entity::declareType(noAttribs,"u64",std::vector<token>({token("primitive64")}));
			defaultSignedIntegerType = Entity::declareType(noAttribs,"i64",std::vector<token>({token("primitive64")}));
			Entity::declareType(noAttribs,"u32",std::vector<token>({token("primitive32")}));
			Entity::declareType(noAttribs,"u16",std::vector<token>({token("primitive16")}));
			Entity::declareType(noAttribs, "u8",std::vector<token>({token( "primitive8")}));
			defaultPointerType = Entity::declareType(noAttribs,"ptr_t",std::vector<token>({token("primitive64")}));
			defaultCharType = Entity::declareType(noAttribs,"char",std::vector<token>({token("primitive8")}));
			defaultWcharType = Entity::declareType(noAttribs,"wchar",std::vector<token>({token("primitive16")}));
			defaultBooleanType = Entity::declareType(noAttribs,"bool",std::vector<token>({token("primitive8")}));
			defaultFloatType = Entity::declareType(noAttribs,"f64",std::vector<token>({token("primitiveFloat64")}));
			Entity::declareType(noAttribs,"f32",std::vector<token>({token("primitiveFloat32")}));
		}
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
            parse::Lines(stdLines,"@default includes");
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
        if(options::vstc || options::vsls)
            __reqFileVSTC = currentFile;
        parse::Lines(lines,i);
        genOutput(i);
        if(options::docDir != "")
            std::filesystem::create_directories(options::docDir);
        for(format* f : oFormats)
        {
            std::string oname = options::docDir+"/"+rname+"."+f->extension;
            //std::cout << "outputting: " << oname << std::endl;
            f->write(1,oname);
        }
    }
	std::cout << "compiler done" << std::endl;
	if(issues::ErrorCount != 0)
		return -1;
    return 0;
}