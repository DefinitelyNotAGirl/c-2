/*
 * Created Date: Monday August 7th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday August 7th 2023 9:10:37 pm
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

#include <compiler.h>

static std::string colorWARN  = "\033[34m";
static std::string colorERROR = "\033[31m";
static std::string colorRESET = "\033[0m";

bool warn(warning* w,token* t, std::string message)
{
    if(!w->enabled)
        return false;//warning disabled, dont give output

    std::string highlight;
    if(w->error)
    {
        std::cout << colorERROR << "ERROR: " << colorRESET;
        highlight = colorERROR;
    }
    else
    {
        std::cout << colorWARN << "WARNING: " << colorRESET;
        highlight = colorWARN;
    }

    std::cout << message;
    std::cout << " [";
    for(std::string& i : w->enablers)
        std::cout << highlight << i << colorRESET << ",";
    if(w->enablers.size() > 0)
        std::cout << '\b';
    std::cout << "]" << std::endl;

    std::string lineNumStr = std::to_string(t->Line->lineNum);
    std::cout << "at line: " << lineNumStr << " in file: "<<t->Line->file<<": ";
    uint64_t arrowPos = t->col+lineNumStr.size()+10+11+2+t->Line->file.size()-2;
    std::cout   << t->Line->text.substr(0,arrowPos) 
                << highlight << t->Line->text.substr(arrowPos,t->Line->text.length()) 
                << colorRESET << std::endl;
    for(uint64_t i = 0; i < arrowPos; i++)
        std::cout << ' ';
    std::cout <<highlight<<"^"<<colorRESET<< std::endl;
    return true;
}
bool warn(warning* w,line* l, std::string message)
{
    if(!w->enabled)
        return false;//warning disabled, dont give output

    std::string highlight;
    if(w->error)
    {
        std::cout << colorERROR << "ERROR: " << colorRESET;
        highlight = colorERROR;
    }
    else
    {
        std::cout << colorWARN << "WARNING: " << colorRESET;
        highlight = colorWARN;
    }

    std::cout << message;
    std::cout << " [";
    for(std::string& i : w->enablers)
        std::cout << highlight << i << colorRESET << ",";
    if(w->enablers.size() > 0)
        std::cout << '\b';
    std::cout << "]" << std::endl;

    std::string lineNumStr = std::to_string(l->lineNum);
    std::cout << "at line: " << lineNumStr << " in file: "<<l->file<<": "<<l->text<<std::endl;
    return true;
}
