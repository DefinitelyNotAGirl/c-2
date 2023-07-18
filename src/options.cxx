/*
 * Created Date: Monday July 10th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday July 10th 2023 4:02:45 pm
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

#include <libCargparse.hxx>
#include <util.h>
#include <options.h>

bool strToBool(std::string str)
{
    while(str.front() == '=')
        str = str.substr(1);
    strToLower(str);

    if(str=="true" || str=="1" || str=="yes" || str=="y" || str=="on" || str=="enable" || str=="enabled")
        return true;
    if(str=="false" || str=="0" || str=="no" || str=="n" || str=="off" || str=="disable" || str=="disabled")
        return false;
    return false;
}

#define CARGHANDLER_BOOLEAN(optname) void cargHandler_bool_ ## optname (CARGPARSE_HANDLER_ARGS){\
    if(args.size()==0)\
        options::optname = true;\
    else\
        options::optname = strToBool(args.front());\
}

CARGHANDLER_BOOLEAN(MD)
CARGHANDLER_BOOLEAN(C)
CARGHANDLER_BOOLEAN(fclasslayout)
CARGHANDLER_BOOLEAN(ffreestanding)
CARGHANDLER_BOOLEAN(fnostdlib)
CARGHANDLER_BOOLEAN(fnolibc)
CARGHANDLER_BOOLEAN(mnortti)
CARGHANDLER_BOOLEAN(dprintTokens)

#define name_MD "-MD"
#define name_C "-C"
#define name_fclasslayout "--fclasslayout"
#define name_ffreestanding "--ffreestanding"
#define name_fnostdlib "--fnostdlib"
#define name_fnolibc "--fnolibc"
#define name_mnortti "--mno-rtti"
#define name_dprintTokens "--dprint-tokens"

#define CARGHANDLER_BOOLEAN_N(optname) cargHandler_bool_ ## optname
#define CARGHANDLER_BOOLEAN_CA0(optname) carg.addParameter(false,0,name_##optname,&CARGHANDLER_BOOLEAN_N(optname));

void cargHandler_unknown(CARGPARSE_UNKOWNHANDLER_ARGS)
{
    sourceFiles.push_back(arg);
}

void cliOptions(int argc, char **argv) 
{
    cargparse::instance carg;
    //misc
    CARGHANDLER_BOOLEAN_CA0(MD);
    CARGHANDLER_BOOLEAN_CA0(C);
    //f
    CARGHANDLER_BOOLEAN_CA0(fclasslayout);
    CARGHANDLER_BOOLEAN_CA0(ffreestanding);
    CARGHANDLER_BOOLEAN_CA0(fnostdlib);
    CARGHANDLER_BOOLEAN_CA0(fnolibc);
    //m
    CARGHANDLER_BOOLEAN_CA0(mnortti);
    //d
    CARGHANDLER_BOOLEAN_CA0(dprintTokens);

    carg.unknownHandler = &cargHandler_unknown;

    carg.parse(argc,argv);
}