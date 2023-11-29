/*
 * Created Date: Sunday September 17th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday September 17th 2023 10:35:30 am
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

void CARGHANDLER_HELP(CARGPARSE_HANDLER_ARGS)
{
    std::cout << "Usage: cp2 [options] file" << std::endl;
    std::cout << "options:"<< std::endl;
    {//help
        printPreOptionSpaces();
        std::cout << "-h, --help";
        CSLLB+=11;printEvenSpaces();
        std::cout << "display this info message.";
        printOptReset();
    }
    {//help-pragma
        printPreOptionSpaces();
        std::cout << "--help-pragma";
        CSLLB+=14;printEvenSpaces();
        std::cout << "display instructions on how to use #pragma in your source code";
        printOptReset();
    }
    {//version
        printPreOptionSpaces();
        std::cout << "-v, -version";
        CSLLB+=13;printEvenSpaces();
        std::cout << "display the version of this compiler";
        printOptReset();
    }
    {//C
        printPreOptionSpaces();
        std::cout << "-c";
        CSLLB+=3;printEvenSpaces();
        std::cout << "emit object file, do not invoke the linker.";
        printOptReset();
    }
    {//MD
        printPreOptionSpaces();
        std::cout << "-MD";
        CSLLB+=4;printEvenSpaces();
        std::cout << "emit make dependency file (*.d) alongside the regular output";
        printOptReset();
    }
    {//o
        printPreOptionSpaces();
        std::cout << "-o <path>";
        CSLLB+=10;printEvenSpaces();
        std::cout << "set <path> as the output destination";
        printOptReset();printEvenSpaces();
        std::cout << "if <path> is a directory, output files are given names %.$";
        printOptReset();printEvenSpaces();
        std::cout << "where % is the base name of the source file";
        printOptReset();printEvenSpaces();
        std::cout << "and $ is the extension of the format";
        printOptReset();printEvenSpaces();
        std::cout << "if <path> is a file, output files are saved as %.$";
        printOptReset();printEvenSpaces();
        std::cout << "where % is <path> with its extension removed, if it has one";
        printOptReset();printEvenSpaces();
        std::cout << "and $ is the extension of the format";
        printOptReset();
    }
    {//S
        printPreOptionSpaces();
        std::cout << "-S";
        CSLLB+=3;printEvenSpaces();
        std::cout << "do not delete assembly code after running the assembler";
        printOptReset();
    }
    {//s
        printPreOptionSpaces();
        std::cout << "-s";
        CSLLB+=3;printEvenSpaces();
        std::cout << "produce neither an object file nor an executable, just the assembly code";
        printOptReset();printEvenSpaces();
        std::cout << "note: using this options with -S and/or -c does not make sense";
        printOptReset();
    }
    {//ffreestanding
        printPreOptionSpaces();
        std::cout << "--ffreestanding";
        CSLLB+=16;printEvenSpaces();
        std::cout << "do not include and link the standard library,";
        printOptReset();printEvenSpaces();
        std::cout << "this option implies the following options: ";
        printOptReset();printEvenSpaces();
        std::cout << "  --fno-stdlib";
        printOptReset();printEvenSpaces();
        std::cout << "  --fno-autoinclude";
        printOptReset();
    }
    {//fno-stdlib
        printPreOptionSpaces();
        std::cout << "--fno-stdlib";
        CSLLB+=13;printEvenSpaces();
        std::cout << "do not add the include directory for the standard library";
        printOptReset();printEvenSpaces();
        std::cout << "note: when using this option you will need to provide your own <stdint> header";
        printOptReset();printEvenSpaces();
        std::cout << "      or define the default data types elsewhere";
        printOptReset();
    }
    {//fno-autoinclude
        printPreOptionSpaces();
        std::cout << "--fno-autoinclude";
        CSLLB+=18;printEvenSpaces();
        std::cout << "do not silently include headers such as <stdint> or <Memory> before begining compilation";
        printOptReset();printEvenSpaces();
        std::cout << "note: when using this option you will need to manually include the <stdint> header";
        printOptReset();printEvenSpaces();
        std::cout << "      or define the default data types elsewhere";
        printOptReset();
    }
    {//fcpl
        printPreOptionSpaces();
        std::cout << "--fcpl <number>";
        CSLLB+=16;printEvenSpaces();
        std::cout << "set the initial cpu privilege level to <number>";
        printOptReset();printEvenSpaces();
        std::cout << "note: this option should simply be left alone for user-space software";
        printOptReset();
    }
    {//vsls
        printPreOptionSpaces();
        std::cout << "--vsls";
        CSLLB+=16;printEvenSpaces();
        std::cout << "makes the compiler emmit output to be used by the vscode c+=2 language server";
        printOptReset();printEvenSpaces();
        std::cout << "note: this option is almost certainly irrelevant to you";
        printOptReset();
    }
    {//vstc
        printPreOptionSpaces();
        std::cout << "--vstc";
        CSLLB+=16;printEvenSpaces();
        std::cout << "makes the compiler emmit output to be used by the vscode c+=2 semantic token provider";
        printOptReset();printEvenSpaces();
        std::cout << "note: this option is almost certainly irrelevant to you";
        printOptReset();
    }
    {//mno-rtti
        printPreOptionSpaces();
        std::cout << "--mno-rtti";
        CSLLB+=11;printEvenSpaces();
        std::cout << "do not add run time type information for classes";
        printOptReset();printEvenSpaces();
        std::cout << "note: this option currently serves no function as rtti features have not yet been added";
        printOptReset();
    }
    {//mno-rodata
        printPreOptionSpaces();
        std::cout << "--mno-rodata";
        CSLLB+=13;printEvenSpaces();
        std::cout << "put data that would otherwise end up in .rodata sections into regular read/write .data sections";
        printOptReset();
    }
    {//keepComments
        printPreOptionSpaces();
        std::cout << "--keepComments";
        CSLLB+=15;printEvenSpaces();
        std::cout << "keep line (//) comments in the output assembly code";
        printOptReset();printEvenSpaces();
        std::cout << "note: using this option only matters when used in conjunction with -S";
        printOptReset();
    }
    {//-g
        printPreOptionSpaces();
        std::cout << "-g";
        CSLLB+=3;printEvenSpaces();
        std::cout << "generate debug symbols";
        printOptReset();
    }
    {//-V
        printPreOptionSpaces();
        std::cout << "-V";
        CSLLB+=3;printEvenSpaces();
        std::cout << "generate occasional comments in the assembly code";
        printOptReset();
    }
    {//-VV
        printPreOptionSpaces();
        std::cout << "-VV";
        CSLLB+=4;printEvenSpaces();
        std::cout << "generate verbose comments in the assembly code";
        printOptReset();
    }
    {//-VVV
        printPreOptionSpaces();
        std::cout << "-VVV";
        CSLLB+=5;printEvenSpaces();
        std::cout << "generate extremely verbose comments in the assembly code";
        printOptReset();printEvenSpaces();
        std::cout << "note: using this option only matters when used in conjunction with -S";
        printOptReset();
    }
    {//B
        printPreOptionSpaces();
        std::cout << "-B <directory>";
        CSLLB+=15;printEvenSpaces();
        std::cout << "specifies the build output directory, this is where things like object files and assembly code would end up";
        printOptReset();printEvenSpaces();
        std::cout << "note: using this option in conjunction with -o makes little sense";
        printOptReset();
    }
    {//D
        printPreOptionSpaces();
        std::cout << "-D <directory>";
        CSLLB+=15;printEvenSpaces();
        std::cout << "specifies the documentation output directory, this is where output produced by --info ends up";
        printOptReset();
    }
    {//I
        printPreOptionSpaces();
        std::cout << "-I <directory>";
        CSLLB+=15;printEvenSpaces();
        std::cout << "add a directory to the list of include directories";
        printOptReset();
    }
    {//info
        printPreOptionSpaces();
        std::cout << "--info <object types>:<formats>";
        CSLLB+=32;printEvenSpaces();
        std::cout << "produce output on the object types in all of the requested formats";
        printOptReset();printEvenSpaces();
        std::cout << "<object types> must be a comma seperated list of 1 or more of the following words: ";
        printOptReset();printEvenSpaces();
        std::cout << "  \"variables\",\"functions\" or \"classes\"";
        printOptReset();printEvenSpaces();
        std::cout << "<formats> must be a comma seperated list of 1 or more of the following format-ids (ids are encased in brackets): ";
        for(format& i : formats)
        {
            printOptReset();printEvenSpaces();
            std::cout << "("<<i.name<<") " << i.desc;
        }
        printOptReset();printEvenSpaces();
        std::cout << "note: this option may be used more than once";
        printOptReset();
    }
    {//-W
        printPreOptionSpaces();
        std::cout << "-W<warning-id>";
        CSLLB+=15;printEvenSpaces();
        std::cout << "enable a warning with the id <warning-id>";
        printOptReset();
    }
    {//-Wno
        printPreOptionSpaces();
        std::cout << "-Wno-<warning-id>";
        CSLLB+=18;printEvenSpaces();
        std::cout << "disable a warning with the id <warning-id>";
        printOptReset();printEvenSpaces();
        std::cout << "note: these options may be used more than once";
        printOptReset();
    }
    {//keepComments
        printPreOptionSpaces();
        std::cout << "--ddebug";
        CSLLB+=9;printEvenSpaces();
        std::cout << "output various kinds of debug information to stdout, used to debug the compiler itself";
        printOptReset();printEvenSpaces();
        std::cout << "note: this option is best left alone unless you are working on the compiler";
        printOptReset();printEvenSpaces();
        std::cout << "      or are asked to use it by someone who is working on the compiler";
        printOptReset();printEvenSpaces();
        std::cout << "      as the information produced by this option would mostly look like";
        printOptReset();printEvenSpaces();
        std::cout << "      useless gibberish to anyone unfamiliar with the compilers inner workings";
        printOptReset();
    }
}
