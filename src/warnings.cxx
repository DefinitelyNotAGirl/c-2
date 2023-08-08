/*
 * Created Date: Monday August 7th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday August 7th 2023 7:55:53 pm
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

std::vector<warning*> warnings;
std::stack<std::vector<warning*>> pragmaStackWarnings;

void enableWarning(std::string name,std::string enabler)
{
    warning* w = getWarning(name);
    if(w != nullptr)
    {
        w->enabled = true;
        w->enablers.push_back(enabler);
    }
    else
    {
        //warning, unknown warning
    }
}

void disableWarning(std::string name)
{
    warning* w = getWarning(name);
    if(w != nullptr)
    {
        w->enabled = false;
    }
    else
    {
        //warning, unknown warning
    }
}

warning* getWarning(std::string name)
{
    for(warning* i : warnings)
        if(i->name == name)
            return i;
    return nullptr;
}

warning::warning(std::string name,std::string link, bool enabled, bool error)
    :name(name),link(link),enabled(enabled),error(error)
    {
        if(enabled)
            this->enablers.push_back(name);
    }

warning::warning(std::string name, bool enabled, bool error)
    :name(name),enabled(enabled),error(error)
    {
        if(enabled)
            this->enablers.push_back(name);
    }
