/**
 * Created Date: Monday July 10th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 18th 2023 4:48:23 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
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
#pragma once

#include <common.h>
#include <compiler.h>
#define defaultSysRoot "/usr/local"

extern std::list<std::string> sourceFiles;
extern uint64_t defaultNumberBase;
extern uint64_t tabLength;
extern ABI* defaultABI;
extern bool asmDebugComments;
extern char c_alert;
extern char c_backspace;
extern char c_escape;
extern char c_formfeedpagebreak;
extern char c_newline;
extern char c_cariagereturn;
extern char c_horizontaltab;
extern char c_verticaltab;
extern std::string objOut;
extern std::string execOut;
extern std::string asmOut;
extern std::string mdOut;
extern std::vector<std::string> includeDirs;

namespace options
{
    //-f*******
    extern bool ffreestanding;
    extern bool fnostdlib;
    extern bool fnolibc;
    extern bool fnoautoinclude;
    extern bool fclasslayout;
    extern bool ffunctioninfo;
    extern bool fvariableinfo;
    extern int fcpl;
    extern int asmVerbose;
    extern bool keepComments;

    //-m******
    extern bool mnortti;
    extern bool mnorodata;

    //-d******
    extern bool dprintTokens;
    extern bool ddebug;
    extern bool asmSepComments;

    //misc
    extern bool MD;
    extern bool C;
    extern bool as;
    extern bool aso;
    extern bool nod;
    extern bool vsls;
    extern bool vstc;
    extern bool debugSymbols;
    extern bool mainAutoSym;
    extern std::string output;
    extern std::string SysRoot;

    extern std::string buildDir;
    extern std::string docDir;
}