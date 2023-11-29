/*
 * Created Date: Sunday July 30th 2023
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

#include <error.h>
#include <compiler.h>

namespace error
{

std::string functionExpr = "";
std::list<std::string> candidateExpressions;

static void genericErrorDisplay(token& t,uint64_t offset = 0)
{
    std::string lineNumStr = std::to_string(t.Line->lineNum);
    std::cout << "at line: " << lineNumStr << " in file: "<<t.Line->file<<": "<<t.Line->text<<  std::endl;
    uint64_t arrowPos = t.col+t.Line->leadingSpaces+lineNumStr.size()+10+11+2+t.Line->file.size()-2+offset;
    for(uint64_t i = 0; i < arrowPos; i++)
        std::cout << ' ';
    std::cout << "\033[31m^\033[0m" << std::endl;
}

void functionNotFound(line& l)
{
    if(options::vsls)
    {
        std::cout << "0001-" << l.lineNum << '-' << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout << "\033[31mERROR:\033[0m no function matching call " << functionExpr << std::endl;
    std::string lineNumStr = std::to_string(l.lineNum);
    std::cout << "at line: " << lineNumStr << " in file: "<<l.file<<": "<<l.text<< std::endl;
    if(candidateExpressions.size() > 0)
    {
        std::cout << "candidates are: " << std::endl;
        for(std::string& i : candidateExpressions)
            std::cout <<"    "<< i << std::endl;
    }
}

//used ids:
//  0x1000
void genericError(uint64_t ID)
{
    if(options::vstc)return;
    if(options::vsls)return;
    std::cout << "\033[31mERROR:\033[0m generic error. code: " <<"0x"<<std::hex<< ID << std::endl;
}

void incompleteType(token& t)
{
    if(options::vsls)
    {
        std::cout << "0002-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout << "\033[31mERROR:\033[0m cannot declare variable with incomplete type \"" << t.text << "\"" << std::endl;
    genericErrorDisplay(t);
}

void noSuchFile(token& t)
{
    if(options::vsls)
    {
        std::cout << "0003-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout << "\033[31mERROR:\033[0m cannot find file \"" << t.text << "\"" << std::endl;
    genericErrorDisplay(t);
}

void compilerBug(std::string file, int line)
{
    if(options::vsls)return;
    if(options::vstc)return;
    std::cout << "\033[31mERROR:\033[0m this is a bug, please open an issue at https://github.com/DefinitelyNotAGirl/c-2/issues and pass on the following information." << std::endl;
    std::cout << "##### debug information start #####" << std::endl;;
    std::cout << "file: " << file << std::endl;
    std::cout << "line: " << line << std::endl;
    std::cout << "head: " << buildCOMMIT << std::endl;
    std::cout << "date: " << buildDATE << std::endl;
    std::cout << "branch: " << buildBRANCH << std::endl;
    std::cout << "##### debug information end   #####" << std::endl;
    note("please tag the issue as \"bug\", thanks!");
    note("feel free to attach the source code that triggered this bug!");
}

void arraySizeImmediate(token& t)
{
    if(options::vsls)
    {
        std::cout << "0004-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout << "\033[31mERROR:\033[0m array size must be an immediate in this context!" << std::endl;
    error::genericErrorDisplay(t);
}

void noSuchABI(token& t)
{
    if(options::vsls)
    {
        std::cout << "0005-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout << "\033[31mERROR:\033[0m ABI \"" << t.text << "\" not found" << std::endl;
    error::genericErrorDisplay(t);
}

void noSuchType(token& t)
{
    if(options::vsls)
    {
        std::cout << "0006-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout << "\033[31mERROR:\033[0m \"" << t.text << "\" does not name a type" << std::endl;
    error::genericErrorDisplay(t);
}

void noSuchIdentifier(token& t)
{
    if(options::vsls)
    {
        std::cout << "0015-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout << "\033[31mERROR:\033[0m \"" << t.text << "\" does not name anything!" << std::endl;
    error::genericErrorDisplay(t);
}

void invalidClassAttribute(token& t)
{
    if(options::vsls)
    {
        std::cout << "0007-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    std::cout << "\033[31mERROR:\033[0m attribute \"" << t.text << "\" is not valid for classes" << std::endl;
    error::genericErrorDisplay(t);
}
void invalidFunctionAttribute(token& t)
{
    if(options::vsls)
    {
        std::cout << "0008-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    std::cout << "\033[31mERROR:\033[0m attribute \"" << t.text << "\" is not valid for functions" << std::endl;
    error::genericErrorDisplay(t);
}
void invalidVariableAttribute(token& t)
{
    if(options::vsls)
    {
        std::cout << "0009-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    std::cout << "\033[31mERROR:\033[0m attribute \"" << t.text << "\" is not valid for variable" << std::endl;
    error::genericErrorDisplay(t);
}

void noSuchLitop(token& t, uint64_t offset = 0)
{
    if(options::vsls)
    {
        std::cout << "0010-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces+offset << '-' << t.text.length()-offset << '\n';
        return;
    }
    std::cout << "\033[31mERROR:\033[0m \"" << t.text.substr(offset,t.text.length()) << "\" does not name a literal operator" << std::endl;
    error::genericErrorDisplay(t,offset);
}

static std::string getTokenTypename(token& t)
{
    switch(t.type)
    {
        case(12):
        case(9):
            return "typename";
        case(2):
            return "immediate";
        case(3):
            return "operator";
        case(6):
        case(7):
            return "string literal";
        case(8):
            return "keyword";
        case(10):
            return "variable name";
        case(1):
            return "new unique identifier";
        case(30):
            return "opening round bracket";
        case(31):
            return "closing round bracket";
        case(32):
            return "opening sqaure bracket";
        case(33):
            return "closing square bracket";
        case(34):
            return "opening angle bracket";
        case(35):
            return "closing angle bracket";
        case(36):
            return "opening curly bracket";
        case(37):
            return "closing curly bracket";
        case(40):
            return "colon";
        case(41):
            return "semicolon";
        case(42):
            return "comma";
    }
    return "INVALID TOKEN TYPE";
}

void expectedNewUnique(token& t)
{
    if(options::vsls)
    {
        std::cout << "0011-" <<'-'<<currentFile<<'-'<< t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout   << "\033[31mERROR:\033[0m expected new unique identifier instead of " 
                << getTokenTypename(t) 
                << " \"" << t.text << "\"" 
                << std::endl;
    error::genericErrorDisplay(t);
}

void expectedTypename(token& t)
{
    if(options::vsls)
    {
        std::cout << "0012-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout   << "\033[31mERROR:\033[0m expected type name instead of " 
                << getTokenTypename(t) 
                << " \"" << t.text << "\"" 
                << std::endl;
    error::genericErrorDisplay(t);
}

void expectedTemplateArg(token& t)
{
    if(options::vsls)
    {
        std::cout << "0013-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout   << "\033[31mERROR:\033[0m expected type name instead of " 
                << getTokenTypename(t) 
                << " \"" << t.text << "\"" 
                << std::endl;
    error::genericErrorDisplay(t);
}

void expectedShortop(token& t)
{
    if(options::vsls)
    {
        std::cout << "0014-" << t.Line->lineNum << '-' << t.col+t.Line->leadingSpaces << '-' << t.text.length() << '-' << t.text << '\n';
        return;
    }
    if(options::vstc)return;
    std::cout   << "\033[31mERROR:\033[0m expected op code instead of " 
                << getTokenTypename(t) 
                << " \"" << t.text << "\"" 
                << std::endl
                << "valid op codes are: \"add\",\"sub\",\"mul\",\"div\" and \"cast\""
                << std::endl;
    error::genericErrorDisplay(t);
}

}//namespace error end