/*
 * Created Date: Thursday August 3rd 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 3rd 2023 1:58:41 pm
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

#include "pops.h"
#include <bits.h>

location::location(__register__ base, uint64_t offset)
    :base(base),offset(offset){}
location::location(uint64_t offset)
    :base(__register__::invalid),offset(offset){}

std::string location::expr()
{
    std::string res;
    if(this->base != __register__::invalid)
    {
        res = "%"+registerNAME(this->base);
        if(!EXPR_GETBIT_63(this->offset))
            this->offset--;
        this->offset^=-1;
        if(EXPR_GETBIT_63(this->offset))
            res+="-";
        else
            res+="+";
        res+="$";
        std::string num = std::to_string((int64_t)this->offset);
        if(num[0] == '-')
            num = num.substr(1,num.length());
        res+=num;
    }
    else
        res+=std::to_string(this->offset);
    return res;
}