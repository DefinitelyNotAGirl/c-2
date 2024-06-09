/*
 * Created Date: Monday December 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday May 22nd 2024 11:30:22 am
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2024 DefinitelyNotAGirl@github
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

#include <resources.hxx>

extern std::vector<std::string> resourceCode;
extern bool emitExceptionSymbols;
extern uint64_t exceptionoffset;
void genOutput(std::string& i)
{
    if(options::vsls || options::vstc)
        return;
    int results;
    FILE* f;
    //output
	//,
	//, write resource file
	//,
	{
		std::string text = "";
		for(std::string& line : resourceCode)
		{
			text+=line+"\n";
		}
		//,
		//, write to file
		//,
		{
			f = fopen(resOut.c_str(),"w");
    		if(f == NULL)
    		{
    		    std::cout << "ERROR: " << strerror(errno) << std::endl;
    		    std::cout << "ERROR: could not open file \"" << resOut << "\"" << std::endl;
    		}
    		results = fwrite(text.c_str(),text.length(),1,f);
    		if (results == EOF)
    		{
    		    std::cout << "ERROR: could write to file \"" << resOut << "\"" << std::endl;
    		}
    		fclose(f);
		}
	}
	//,
	//, generate dependency file
	//,
    if(options::MD)
    {
        f = fopen(mdOut.c_str(),"w");
        if(f == NULL)
        {
            std::cout << "ERROR: could not open file \"" << mdOut << "\"" << std::endl;
            goto endAsmOutput;
        }
        std::string content;
        if(!options::C)
            content = execOut+":";
        else
            content = objOut+":";
        for(std::string& dep : dependencies)
            content+=" "+dep;
        if(!options::C)
            for(std::string& std : startObjFiles)
                content+=" "+std;
        if(!options::C)
            if(!options::ffreestanding)
                content+=" /usr/lib/libcpe2.a";
        results = fwrite(content.c_str(),content.length(),1,f);
        if (results == EOF)
        {
            std::cout << "ERROR: could write to file \"" << mdOut << "\"" << std::endl;
            goto endAsmOutput;
        }
        fclose(f);
    }
    endAsmOutput:;
	//,
	//, invoke linker (if not -c and not -s)
	//,
    if(!options::C && !options::aso)
    {
        //std::string LDcmd = "ld -no-pie -o a.exe "+objOut;
        std::string LDcmd = "ld -o "+execOut+" "+objOut;
        for(std::string& i : startObjFiles)
            LDcmd+=" "+i;
        if(!options::ffreestanding)
            LDcmd+=" -lcpe2";
        if(options::ddebug)
            std::cout << "ld: " << LDcmd  << std::endl;
        system(LDcmd.c_str());
    }
    //remove unwanted output
    if(!options::as && !options::aso)
        remove(asmOut.c_str());
    if(!options::C)
        remove(objOut.c_str());
}
