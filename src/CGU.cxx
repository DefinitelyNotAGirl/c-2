/*
 * Created Date: Tuesday June 4th 2024
 * Author: Lilith
 * -----
 * Last Modified: Sat Jun 29 2024
 * Modified By: Lilith
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
/** @file */

#include <compiler.h>
#include <class_variable.h>
#include <amd64.cgu.hxx>

namespace runtime
{
	/**
	 * @brief copies the data from src to dst
	 * 
	 * @param src 
	 * @param dst 
	 */
	void copy(variable* src, variable* dst)
	{
		amd64::copy(src,dst);
	}

	/**
	 * @brief clears all bytes in target to zero
	 * 
	 * @param target 
	 */
	void clear(variable* target)
	{
		amd64::clear(target);
	}

	/**
	 * @brief starts a new thread which will run the code at symbol
	 * 
	 * @param symbol 
	 */
	void thread(std::string& symbol)
	{
		amd64::thread(symbol);
	}

	/**
	 * @brief calls func, this does not pass any arguments
	 * 
	 * @param func 
	 */
	void call(function* func)
	{
		amd64::call(func);
	}

	/**
	 * @brief creates a new stack frame of size frameSize
	 * 
	 * @param frameSize
	 */
	void enter(uint64_t frameSize)
	{
		amd64::enter(frameSize);
	}

	/**
	 * @brief destroys the current stack frame
	 * 
	 */
	void leave()
	{
		amd64::leave();
	}

	/**
	 * @brief performs unsigned integer addition
	 * 
	 * @param a first input
	 * @param b second input
	 * @return variable holding the result
	 */
	variable* UnsignedIntegerAddition(variable* a, variable* b)
	{
		return amd64::UnsignedIntegerAddition(a, b);
	}
	
	void RelativeControlTransfer(ImmediateValue offset)
	{
		return amd64::RelativeControlTransfer(offset);
	}
	void AbsoluteControlTransfer(ImmediateValue address)
	{
		return amd64::AbsoluteControlTransfer(address);
	}

	/**
	 * @brief save all user registers to the stack except the stack pointer, the frame pointer and the thread-data pointer
	 * @return stack frame offset of saved data
	 */
	uint64_t SaveAll()
	{
		return amd64::SaveAll();
	}

	/**
	 * @brief load all user registers from the stack except the stack pointer, the frame pointer and the thread-data pointer
	 */
	void LoadAll(uint64_t offset)
	{
		return amd64::LoadAll(offset);
	}
}