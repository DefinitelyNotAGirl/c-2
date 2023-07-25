/*
 * Created Date: Monday July 24th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday July 24th 2023 6:56:02 am
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

#include <common.h>
#include <compiler.hxx>
#include <util.h>

codechar* manglePseudoName(codechar* name)
{
    if(cstrcmp(name,"operator++"))
        return "operatorINC";
    if(cstrcmp(name,"operator--"))
        return "operatorDEC";
    if(cstrcmp(name,"operator+"))
        return "operatorADD";
    if(cstrcmp(name,"operator+="))
        return "operatorADDA";
    if(cstrcmp(name,"operator-"))
        return "operatorSUB";
    if(cstrcmp(name,"operator-="))
        return "operatorSUBA";
    if(cstrcmp(name,"operator*"))
        return "operatorMUL";
    if(cstrcmp(name,"operator*="))
        return "operatorMULA";
    if(cstrcmp(name,"operator/"))
        return "operatorDIV";
    if(cstrcmp(name,"operator/="))
        return "operatorDIVA";
    if(cstrcmp(name,"operator%"))
        return "operatorMOD";
    if(cstrcmp(name,"operator%="))
        return "operatorMODA";
    if(cstrcmp(name,"operator&"))
        return "operatorAND";
    if(cstrcmp(name,"operator&&"))
        return "operatorANDD";
    if(cstrcmp(name,"operator|"))
        return "operatorOR";
    if(cstrcmp(name,"operator||"))
        return "operatorORD";
    if(cstrcmp(name,"operator^"))
        return "operatorEXP";
    return name;
}