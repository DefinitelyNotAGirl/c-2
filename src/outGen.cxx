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
    if(options::vsls || options::vstc)
        return;
    int results;
    FILE* f;
    //output
    std::string asmCode;
    asmCode+="// c2o\n";
    asmCode+="// @syntax gas\n";
    asmCode+="// @file "+i+"\n";
    asmCode+="\n";
    for(std::string& l : MiscCode)
        asmCode+=l+"\n";
    asmCode+=".data\n";
    for(std::string& l : DataCode)
        asmCode+=l+"\n";
    if(options::mnorodata)
        asmCode+=".data\n";
    else
        asmCode+=".section .rodata\n";
    for(std::string& l : RoDataCode)
        asmCode+=l+"\n";
    asmCode+=".bss\n";
    for(std::string& l : BssCode)
        asmCode+=l+"\n";
    asmCode+=".text\n";
    for(std::string& l : TextCode)
        asmCode+=l+"\n";
    if(true /*check for GAS (true for now)*/ && options::debugSymbols)
    {
        asmCode+=".section .debug_info,\"\",@progbits\n";
        for(std::string& l : DebugCode)
            asmCode+=l+"\n";
        asmCode+=".section .debug_abbrev,\"\",@progbits\n";
        asmCode+="debugAbbrev:\n";
        for(std::string& l : DebugAbbrevCode)
            asmCode+=l+"\n";
    }
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
    if(options::MD)
    {
        f = fopen(mdOut.c_str(),"w");
        if(f == NULL)
        {
            std::cout << "ERROR: could not open file \"" << asmOut << "\"" << std::endl;
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
            std::cout << "ERROR: could write to file \"" << asmOut << "\"" << std::endl;
            goto endAsmOutput;
        }
        fclose(f);
    }
    endAsmOutput:;
    //invoke assembler
    std::string ASMcmd = "as --gstabs -o "+objOut+" "+asmOut;
    if(options::ddebug && !options::aso)
        std::cout << "as: " << ASMcmd  << std::endl;
    if(!options::aso)
        system(ASMcmd.c_str());
    //invoke linker (if not -c and not -s)
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
    if(!options::as)
        remove(asmOut.c_str());
    if(!options::C)
        remove(objOut.c_str());
}
