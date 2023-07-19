/**
 * Created Date: Monday July 10th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday July 10th 2023 4:26:13 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
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

void strToLower(std::string& str)
{
    char* buff = (char*)str.c_str();
    for(uint64_t i = 0;i<str.size();i++)
        if(buff[i] >= 'A' && buff[i] <= 'Z')
            buff[i]+=0x20;
}

void strToLower(codechar* buff)
{
    for(uint64_t i = 0;i<strlen(buff);i++)
        if(buff[i] >= 'A' && buff[i] <= 'Z')
            buff[i]+=0x20;
}

uint64_t strlen(codechar* str)
{
    uint64_t len = 0;
    while(*str != 0)
    {
        str+=sizeof(codechar);
        len++;
    }
    return len;
}

bool cstrcmp(codechar* a, codechar* b)
{
    uint64_t len = strlen(a);
    if(len != strlen(b))
        return 0;
    for(uint64_t i = 0;i<len;i++)
        if(a[i] != b[i])
            return 0;
    return 1;
}

bool isDigit(codechar c)
{
    if(c >= '0' && c <= '9')
        return 1;
    return 0;
}
bool isLatinChar(codechar c)
{
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
        return 1;
    return 0;
}

bool strContains(codechar* buff, codechar c)
{
    for(uint64_t i = 0;i<strlen(buff);i++)
        if(buff[i] == c)
            return 1;
    return 0;
}