/*
 * Created Date: Saturday June 8th 2024
 * Author: Lilith
 * -----
 * Last Modified: Saturday June 8th 2024 11:54:00 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2024 DefinitelyNotAGirl@github
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
#include <SMU.h>
#include <amd64.cgu.hxx>
#include <CGU.AMD64.h>

namespace testing 
{
	void main()
	{
		std::cout << ">>>>>>>>>>>> TEST BUILD RUNNING <<<<<<<<<<<<" << std::endl;
		//.
		//. required sections
		//.
		section* text;
		section* data;
		section* rodata;
		//.
		//. data types
		//.
		type* u64 = new type;
		u64->size = 8;
		u64->mangledName = "u64";
		u64->name = "u64";
	}
}