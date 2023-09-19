/*
 * Created Date: Thursday August 3rd 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday September 17th 2023 5:19:35 am
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

void genOutput(std::string& i)
{
    int results;
    FILE* f;
    //output
    std::string asmCode;
    asmCode+="# cpe2\n";
    asmCode+="\n";
    for(std::string& l : MiscCode)
        asmCode+=l+"\n";
    asmCode+="\n.data\n";
    for(std::string& l : DataCode)
        asmCode+=l+"\n";
    if(options::mnorodata)
        asmCode+="\n.data\n";
    else
        asmCode+="\n.section .rodata\n";
    for(std::string& l : RoDataCode)
        asmCode+=l+"\n";
    asmCode+="\n.bss\n";
    for(std::string& l : BssCode)
        asmCode+=l+"\n";
    asmCode+="\n.text\n";
    for(std::string& l : TextCode)
        asmCode+=l+"\n";
    f = fopen(asmOut.c_str(),"w");
    if(f == NULL)
    {
        std::cout << "ERROR: could not open file \"" << asmOut << "\"" << std::endl;
        goto endAsmOutput;
    }
    results = fwrite(asmCode.c_str(),asmCode.length(),1,f);
    if (results == EOF)
    {
        std::cout << "ERROR: could write to file \"" << asmOut << "\"" << std::endl;
        goto endAsmOutput;
    }
    fclose(f);
    endAsmOutput:;
    //invoke assembler
    std::string ASMcmd = "as -o "+objOut+" "+asmOut;
    system(ASMcmd.c_str());
    //invoke linker (if not -c)
    if(!options::C)
    {
        std::string LDcmd = "ld -o a.exe "+objOut;
        system(LDcmd.c_str());
    }
    //remove unwanted output
    if(!options::as)
        remove(asmOut.c_str());
    if(!options::C)
        remove(objOut.c_str());
}