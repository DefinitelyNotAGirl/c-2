/*
 * Created Date: Wednesday July 5th 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday July 5th 2023 3:51:28 am
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

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>

template<typename T>
class revstack
{
private:
    std::vector<T> stack;
public:
    revstack(){}

    void push(T ptr)
    {
        this->stack.push_back(ptr);
    }

    void pop()
    {
        this->stack.pop_back();
    }

    uint64_t size()
    {
        return this->stack.size();
    }

    T operator[](uint64_t index)
    {
        return this->stack[this->stack.size()-1-index];
    }
};
