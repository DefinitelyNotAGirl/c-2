/*
 * Created Date: Tuesday August 8th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday August 8th 2023 12:26:54 pm
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
#include <stack>

std::stack<uint64_t> stack_defaultNumberBase;
std::stack<uint64_t> stack_tabLength;
std::stack<std::vector<warning*>> pragmaStackWarnings;
std::stack<ABI*> stack_defaultABI;
std::stack<bool> stack_fclasslayout;
std::stack<bool> stack_ffunctioninfo;
std::stack<bool> stack_fvariableinfo;
std::stack<bool> stack_fcpl;
std::stack<int> stack_asmVerbose;
std::stack<bool> stack_mnortti;
std::stack<bool> stack_mnorodata;
std::stack<bool> stack_dprintTokens;
std::stack<bool> stack_ddebug;

namespace pragma 
{
    void push()
    {
        pragmaStackWarnings.push(warnings);
        for(warning*& i : warnings)
            i = new warning(*i);

        stack_defaultNumberBase.push(defaultNumberBase);
        stack_tabLength.push(tabLength);
        stack_defaultABI.push(defaultABI);

        stack_fclasslayout.push(options::fclasslayout);
        stack_ffunctioninfo.push(options::ffunctioninfo);
        stack_fvariableinfo.push(options::fvariableinfo);
        stack_fcpl.push(options::fcpl);
        stack_asmVerbose.push(options::asmVerbose);
        stack_mnortti.push(options::mnortti);
        stack_mnorodata.push(options::mnorodata);
        stack_dprintTokens.push(options::dprintTokens);
        stack_ddebug.push(options::ddebug);
    }

    void pop()
    {
        for(warning* i : warnings)
            delete i;
        warnings = pragmaStackWarnings.top();
        pragmaStackWarnings.pop();

        defaultNumberBase = stack_defaultNumberBase.top();
        stack_defaultNumberBase.pop();
        tabLength = stack_tabLength.top();
        stack_tabLength.pop();
        defaultABI = stack_defaultABI.top();
        stack_defaultABI.pop();

        options::fclasslayout = stack_fclasslayout.top();
        stack_fclasslayout.pop();
        options::ffunctioninfo = stack_ffunctioninfo.top();
        stack_ffunctioninfo.pop();
        options::fvariableinfo = stack_fvariableinfo.top();
        stack_fvariableinfo.pop();
        options::fcpl = stack_fcpl.top();
        stack_fcpl.pop();
        options::asmVerbose = stack_asmVerbose.top();
        stack_asmVerbose.pop();
        options::mnortti = stack_mnortti.top();
        stack_mnortti.pop();
        options::mnorodata = stack_mnorodata.top();
        stack_mnorodata.pop();
        options::dprintTokens = stack_dprintTokens.top();
        stack_dprintTokens.pop();
        options::ddebug = stack_ddebug.top();
        stack_ddebug.pop();
    }
}