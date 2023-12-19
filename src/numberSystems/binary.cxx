/*
 * Created Date: Tuesday December 19th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday December 19th 2023 9:16:59 am
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

#define constructor __attribute__ ((constructor))

#include <numberSystem.h>
#include <bits.h>

static uint64_t resolveNumber(std::string& text, uint64_t* numlen)
{
    uint64_t len = text.length();
    uint64_t i = 0;
    uint64_t value = 0;
    for(i=i;i<len;i++)
    {
        switch(text[i])
        {
            case('0'):
                value<<=1;
                break;
            case('1'):
                value<<=1;
                SETBIT_00(value);
                break;
            default:
                goto endSwitch;
        }
    }
    endSwitch:;
    *numlen = i;
    return value;
}

constructor static void init()
{
    numberSystemNames['b'] = "binary";
    numberSystems['b'] = resolveNumber;
}

