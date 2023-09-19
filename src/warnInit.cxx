/*
 * Created Date: Monday August 7th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday August 7th 2023 9:50:24 pm
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

void initWarnings()
{
    //cpl
    warnings.push_back(new warning("cpl-registers",true,true));

    //memory
    warnings.push_back(new warning("memory-absolute",true,false));

    //unused
    warnings.push_back(new warning("unused-variable",true,false));
    warnings.push_back(new warning("unused-class",true,false));

    //return
    warnings.push_back(new warning("missing-return",true,false));
    warnings.push_back(new warning("unexpected-return",true,false));

    //misc
    warnings.push_back(new warning("deprecated",true,false));

    //stack
    warnings.push_back(new warning("stack-pointer-rsp-storage",true,false));
}
