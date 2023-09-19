/*
 * Created Date: Sunday September 17th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday September 17th 2023 1:01:56 pm
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

#include <libCargparse.hxx>
#include <util.h>
#include <options.h>
#include <compiler.h>
#include <miscout.h>

static uint64_t CSLLB = 0;

static void printPreOptionSpacesDouble()
{
    CSLLB+=4;
    std::cout << "    ";
}

static void printPreOptionSpaces()
{
    CSLLB+=2;
    std::cout << "  ";
}

static void printOptReset()
{
    CSLLB=0;
    std::cout << std::endl;
}

static void printEvenSpaces()
{
    if(CSLLB == 0)
        CSLLB = 1;
    for(uint64_t i = CSLLB;i<60;i++)
        std::cout << ' ';
}

void CARGHANDLER_HELP_PRAGMA(CARGPARSE_HANDLER_ARGS)
{
    std::cout << "#pragma directives can be used to manipulate the compilers behavior in a number of ways," << std::endl;
    std::cout << "the following manual lines out how to use #pragma directives." << std::endl;
    std::cout << "note: there may be undocumented #pragma options,"
              << " the behaviour and/or availability of such options may change without notice" << std::endl
              << "and/or without any mention in the patch notes" << std::endl;
    std::cout << "#pragma is always followed up by one of the following top level options:" << std::endl;
    {//push
        printPreOptionSpaces();
        std::cout << "push";
        CSLLB+=5;printEvenSpaces();
        std::cout << "push the compilers internal state for later restoration";
        printOptReset();
    }
    {//pop
        printPreOptionSpaces();
        std::cout << "pop";
        CSLLB+=4;printEvenSpaces();
        std::cout << "restore the compilers internal state from the stack";
        printOptReset();printEvenSpaces();
        std::cout << "note: restoring the state without pushing it prior will cause the compiler to crash (signal 11, segmentation fault)";
        printOptReset();
    }
    {//warning
        printPreOptionSpaces();
        std::cout << "warning";
        CSLLB+=8;printEvenSpaces();
        std::cout << "controls how/if warnings are emitted";
        printOptReset();
    }
    {//cpl
        printPreOptionSpaces();
        std::cout << "cpl";
        CSLLB+=4;printEvenSpaces();
        std::cout << "controls the cpu privilege level assumed by the compiler";
        printOptReset();
    }
    {//ABI
        printPreOptionSpaces();
        std::cout << "ABI";
        CSLLB+=4;printEvenSpaces();
        std::cout << "controls the default ABI";
        printOptReset();
    }
    {//once
        printPreOptionSpaces();
        std::cout << "once";
        CSLLB+=5;printEvenSpaces();
        std::cout << "prevents the current file from being included more then once";
        printOptReset();
    }
    {//stack-pointer
        printPreOptionSpaces();
        std::cout << "stack-pointer";
        CSLLB+=14;printEvenSpaces();
        std::cout << "controls which register is used to store the stack pointer";
        printOptReset();
    }
    std::cout << "now follow detailed explanations on how to use the aforementioned options." << std::endl;
    {//warning
        printPreOptionSpaces();
        std::cout << "warning";
        CSLLB+=8;printEvenSpaces();
        std::cout << "is followed by one of the following options";
        printOptReset();
        {//enable
            printPreOptionSpacesDouble();
            std::cout << "enable";
            CSLLB+=7;printEvenSpaces();
            std::cout << "  enables the warning with the warning-id following this option";
            printOptReset();
        }
        {//disable
            printPreOptionSpacesDouble();
            std::cout << "disable";
            CSLLB+=8;printEvenSpaces();
            std::cout << "  disables the warning with the warning-id following this option";
            printOptReset();
        }
    }
    {//cpl
        printPreOptionSpaces();
        std::cout << "cpl";
        CSLLB+=4;printEvenSpaces();
        std::cout << "sets the cpu privilege level assumed by the compiler to the number following this option";
        printOptReset();printEvenSpaces();
        std::cout << "range: 0-255";
        printOptReset();
    }
    {//stack-pointer
        printPreOptionSpaces();
        std::cout << "stack-pointer";
        CSLLB+=14;printEvenSpaces();
        std::cout << "sets the register currently used as the stack pointer to the register the name of which is equal to the name following this option";
        printOptReset();printEvenSpaces();
        std::cout << "note: do not use this option, it will cause issues";
        printOptReset();
    }
    {//ABI
        printPreOptionSpaces();
        std::cout << "ABI";
        CSLLB+=4;printEvenSpaces();
        std::cout << "sets the default ABI to the abi with the name that follows this option";
        printOptReset();printEvenSpaces();
        std::cout << "available ABIs:";
        for(ABI* i : ABIs)
        {
            printOptReset();printEvenSpaces();
            std::cout << "  " << i->name;
            if(i->name == defaultABI->name)
                std::cout << " (current default)";
        }
        printOptReset();
    }
}
