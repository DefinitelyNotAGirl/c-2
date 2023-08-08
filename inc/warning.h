/**
 * Created Date: Monday August 7th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday August 7th 2023 7:56:39 pm
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
#pragma once

#include <compiler.h>

class warning
{
public:
    warning(std::string name,std::string link, bool enabled, bool error);
    warning(std::string name, bool enabled, bool error);
    std::string name;
    std::vector<std::string> enablers;
    std::string link;//link to documentation page
    bool enabled = false;
    bool error = false;
};

extern std::vector<warning*> warnings;
extern std::stack<std::vector<warning*>> pragmaStackWarnings;
bool warn(warning* w,token* t, std::string message);
bool warn(warning* w,line* l, std::string message);
warning* getWarning(std::string name);
void disableWarning(std::string name);
void enableWarning(std::string name,std::string enabler);
void enableWarningSet(std::string name);
void disableWarningSet(std::string name);
void note(std::string msg);
