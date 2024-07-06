/**
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday June 3rd 2024 11:44:37 pm
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
#pragma once

#include <string>
#include <extint.hxx>
#include <class_variable.h>

class line;

class token
{
public:
	token(){}
	token(variable* var)
	{
		this->text = var->name;
		this->type = 10;
	}
    line* Line = nullptr;
    std::string text;
    uint64_t col = 0;
	uint64_t tcol = 0;
    uint64_t type = 0;
    uint64_t lineNum = 0;

	static std::string Typename(uint64_t t)
	{
    	switch(t)
    	{
    	    case(12):
    	    case(9):
    	        return "typename";
    	    case(2):
    	        return "immediate";
    	    case(3):
    	        return "operator";
    	    case(6):
    	    case(7):
    	        return "string literal";
    	    case(8):
    	        return "keyword";
    	    case(10):
    	        return "variable name";
			case(11):
    	        return "function name";
    	    case(1):
    	        return "new unique identifier";
    	    case(30):
    	        return "opening round bracket";
    	    case(31):
    	        return "closing round bracket";
    	    case(32):
    	        return "opening sqaure bracket";
    	    case(33):
    	        return "closing square bracket";
    	    case(34):
    	        return "opening angle bracket";
    	    case(35):
    	        return "closing angle bracket";
    	    case(36):
    	        return "opening curly bracket";
    	    case(37):
    	        return "closing curly bracket";
    	    case(40):
    	        return "colon";
    	    case(41):
    	        return "semicolon";
    	    case(42):
    	        return "comma";
    	}
    	return "INVALID TOKEN TYPE ("+std::to_string(t)+")";
	}

	std::string Typename()
	{
    	return token::Typename(this->type);
	}
};