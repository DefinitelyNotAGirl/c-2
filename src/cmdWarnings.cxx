/*
 * Created Date: Monday August 7th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday August 7th 2023 8:13:36 pm
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

void enableWarningSet(std::string name)
{
    if(name == "unused")
    {
        enableWarning("unused-variable",name);
        enableWarning("unused-class",name);
    }
    else if(name == "cpl")
    {
        enableWarning("cpl-registers",name);
    }
    else if(name == "all")
    {
        for(warning* i : warnings)
        {
            i->enabled = true;
            i->enablers.push_back(name);
        }
    }
    else
        enableWarning(name,name);
}

void disableWarningSet(std::string name)
{
    if(name == "unused")
    {
        disableWarning("unused-variable");
        disableWarning("unused-class");
    }
    else if(name == "cpl")
    {
        disableWarning("cpl-registers");
    }
    else if(name == "all")
    {
        for(warning* i : warnings)
        {
            i->enabled = false;
        }
    }
    else
        disableWarning(name);
}
