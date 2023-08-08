/*
 * Created Date: Sunday July 30th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday July 30th 2023 4:55:06 am
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

namespace error
{

static void genericErrorDisplay(token& t,uint64_t offset = 0)
{
    std::string lineNumStr = std::to_string(t.Line->lineNum);
    std::cout << "at line: " << lineNumStr << " in file: "<<t.Line->file<<": "<<t.Line->text<<  std::endl;
    uint64_t arrowPos = t.col+lineNumStr.size()+10+11+2+t.Line->file.size()-2+offset;
    for(uint64_t i = 0; i < arrowPos; i++)
        std::cout << ' ';
    std::cout << "\033[31m^\033[0m" << std::endl;
}

void compilerBug(std::string file, int line)
{
    std::cout << "\033[31mERROR:\033[0m this is a bug, please open an issue at https://github.com/DefinitelyNotAGirl/c-2/issues and pass on the following information." << std::endl;
    std::cout << "##### debug information start #####" << std::endl;;
    std::cout << "file: " << file << std::endl;
    std::cout << "line: " << line << std::endl;
    std::cout << "head: " << buildCOMMIT << std::endl;
    std::cout << "date: " << buildDATE << std::endl;
    std::cout << "branch: " << buildBRANCH << std::endl;
    std::cout << "##### debug information end   #####" << std::endl;
    note("please tag the issue as \"bug\", thanks!");
}

void noSuchType(token& t)
{
    std::cout << "\033[31mERROR:\033[0m \"" << t.text << "\" does not name a type" << std::endl;
    error::genericErrorDisplay(t);
}

void invalidFunctionAttribute(token& t)
{
    std::cout << "\033[31mERROR:\033[0m attribute \"" << t.text << "\" is not valid for functions" << std::endl;
    error::genericErrorDisplay(t);
}
void invalidVariableAttribute(token& t)
{
    std::cout << "\033[31mERROR:\033[0m attribute \"" << t.text << "\" is not valid for variable" << std::endl;
    error::genericErrorDisplay(t);
}

void noSuchLitop(token& t, uint64_t offset = 0)
{
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
    }
    return "INVALID TOKEN TYPE";
}

void expectedNewUnique(token& t)
{
    std::cout   << "\033[31mERROR:\033[0m expected new unique identifier instead of " 
                << getTokenTypename(t) 
                << " \"" << t.text << "\"" 
                << std::endl;
    error::genericErrorDisplay(t);
}

void expectedTypename(token& t)
{
    std::cout   << "\033[31mERROR:\033[0m expected type name instead of " 
                << getTokenTypename(t) 
                << " \"" << t.text << "\"" 
                << std::endl;
    error::genericErrorDisplay(t);
}

void expectedShortop(token& t)
{
    std::cout   << "\033[31mERROR:\033[0m expected op code instead of " 
                << getTokenTypename(t) 
                << " \"" << t.text << "\"" 
                << std::endl
                << "valid op codes are: \"add\",\"sub\",\"mul\",\"div\" and \"cast\""
                << std::endl;
    error::genericErrorDisplay(t);
}

}//namespace error end