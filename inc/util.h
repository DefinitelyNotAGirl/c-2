/**
 * Created Date: Monday July 10th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday July 10th 2023 4:17:52 pm
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
#pragma once

#include <string>
#include <common.h>

void strToLower(std::string& str);
bool cstrcmp(codechar* a, codechar* b);
uint64_t strlen(codechar* str);
void strToLower(codechar* buff);

template<typename T>
void inject(std::vector<T>& src, std::vector<T>& dst, uint64_t offset)
{
    std::vector<T> old = dst;

    dst.clear();

    for(uint64_t i = 0;i<offset;i++)
        dst.push_back(old[i]);

    for(T& i : src)
        dst.push_back(i);
    
    for(uint64_t i = offset;i<old.size();i++)
        dst.push_back(old[i]);
}
