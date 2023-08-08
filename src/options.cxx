/*
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

#include <libCargparse.hxx>
#include <util.h>
#include <options.h>
#include <compiler.h>

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
#define CARGHANDLER_PATH(optname) void cargHandler_path_ ## optname (CARGPARSE_HANDLER_ARGS){\
    std::string dir = args.front();\
    while(dir.back() == '/')\
        dir.pop_back();\
    options::optname = dir;\
}
#define CARGHANDLER_STRING(optname) void cargHandler_path_ ## optname (CARGPARSE_HANDLER_ARGS){\
}
#define CARGHANDLER_INT(optname) void cargHandler_int_ ## optname (CARGPARSE_HANDLER_ARGS){\
    options::optname = atoi(args.front().c_str());\
}

void CARGHANDLER_WNO(CARGPARSE_HANDLER_ARGS)
{
    std::string WID = args.front();
    disableWarningSet(WID);
}
void CARGHANDLER_W(CARGPARSE_HANDLER_ARGS)
{
    std::string WID = args.front();
    enableWarningSet(WID);
}

void CARGHANDLER_V(CARGPARSE_HANDLER_ARGS){options::asmVerbose = 1;}
void CARGHANDLER_VV(CARGPARSE_HANDLER_ARGS){options::asmVerbose = 2;}
void CARGHANDLER_VVV(CARGPARSE_HANDLER_ARGS){options::asmVerbose = 3;}

CARGHANDLER_BOOLEAN(MD)
CARGHANDLER_BOOLEAN(C)
CARGHANDLER_BOOLEAN(as)
CARGHANDLER_PATH(SysRoot)
CARGHANDLER_PATH(output)
CARGHANDLER_BOOLEAN(fclasslayout)
CARGHANDLER_BOOLEAN(ffunctioninfo)
CARGHANDLER_BOOLEAN(fvariableinfo)
CARGHANDLER_BOOLEAN(ffreestanding)
CARGHANDLER_BOOLEAN(fnoautoinclude)
CARGHANDLER_BOOLEAN(fnostdlib)
CARGHANDLER_BOOLEAN(fnolibc)
CARGHANDLER_BOOLEAN(mnortti)
CARGHANDLER_BOOLEAN(mnorodata)
CARGHANDLER_BOOLEAN(dprintTokens)
CARGHANDLER_BOOLEAN(ddebug)
CARGHANDLER_INT(fcpl)

#define name_MD "-MD"
#define name_C "-C"
#define name_output "-o"
#define name_as "-S"
#define name_fclasslayout "--fclassinfo"
#define name_ffunctioninfo "--ffunctioninfo"
#define name_fvariableinfo "--fvariableinfo"
#define name_ffreestanding "--ffreestanding"
#define name_fnoautoinclude "--fno-autoinclude"
#define name_fnostdlib "--fno-stdlib"
#define name_fnolibc "--fno-libc"
#define name_mnortti "--mno-rtti"
#define name_mnorodata "--mno-rodata"
#define name_dprintTokens "--dprint-tokens"
#define name_ddebug "--ddebug"
#define name_SysRoot "--isysroot"
#define name_fcpl "--fcpl"

#define CARGHANDLER_BOOLEAN_N(optname) cargHandler_bool_ ## optname
#define CARGHANDLER_PATH_N(optname) cargHandler_path_ ## optname
#define CARGHANDLER_INT_N(optname) cargHandler_int_ ## optname
#define CARGHANDLER_BOOLEAN_CA0(optname) carg.addParameter(false,0,name_##optname,&CARGHANDLER_BOOLEAN_N(optname));
#define CARGHANDLER_PATH_CA1(optname) carg.addParameter(false,1,name_##optname,&CARGHANDLER_PATH_N(optname));
#define CARGHANDLER_INT_CA1(optname) carg.addParameter(false,1,name_##optname,&CARGHANDLER_INT_N(optname));

void cargHandler_unknown(CARGPARSE_UNKOWNHANDLER_ARGS)
{
    sourceFiles.push_back(arg);
}

void CARGHANDLER_VERSION(CARGPARSE_HANDLER_ARGS)
{
    std::cout << "c+=2 compiler reference implementation." << std::endl;
    std::cout << "version: 0" << std::endl;
    std::cout << "branch: development" << std::endl;
    std::cout << "source: https://github.com/DefinitelyNotAGirl/c-2" << std::endl;

    exit(0);
}

void cliOptions(int argc, char **argv) 
{
    cargparse::instance carg;
    //misc
    CARGHANDLER_BOOLEAN_CA0(MD);
    CARGHANDLER_BOOLEAN_CA0(C);
    CARGHANDLER_BOOLEAN_CA0(as);
    CARGHANDLER_PATH_CA1(output);
    CARGHANDLER_PATH_CA1(SysRoot);
    //f
    CARGHANDLER_BOOLEAN_CA0(fclasslayout);
    CARGHANDLER_BOOLEAN_CA0(ffunctioninfo);
    CARGHANDLER_BOOLEAN_CA0(fvariableinfo);
    CARGHANDLER_BOOLEAN_CA0(ffreestanding);
    CARGHANDLER_BOOLEAN_CA0(fnoautoinclude);
    CARGHANDLER_BOOLEAN_CA0(fnostdlib);
    CARGHANDLER_BOOLEAN_CA0(fnostdlib);
    CARGHANDLER_BOOLEAN_CA0(fnolibc);
    CARGHANDLER_INT_CA1(fcpl);
    //m
    CARGHANDLER_BOOLEAN_CA0(mnortti);
    CARGHANDLER_BOOLEAN_CA0(mnorodata);
    //d
    CARGHANDLER_BOOLEAN_CA0(dprintTokens);
    CARGHANDLER_BOOLEAN_CA0(ddebug);

    carg.addParameter(0,1,"-Wno-",&CARGHANDLER_WNO);
    carg.addParameter(0,1,"-W",&CARGHANDLER_W);

    carg.addParameter(0,0,"-VVV",&CARGHANDLER_VVV);
    carg.addParameter(0,0,"-VV",&CARGHANDLER_VV);
    carg.addParameter(0,0,"-V",&CARGHANDLER_V);

    carg.addParameter(0,0,"-version",&CARGHANDLER_VERSION);
    carg.addParameter(0,0,"-v",&CARGHANDLER_VERSION);

    carg.unknownHandler = &cargHandler_unknown;

    carg.parse(argc,argv);
}