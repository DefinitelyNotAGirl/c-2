/**
 * Created Date: Tuesday June 4th 2024
 * Author: Lilith
 * -----
 * Last Modified: Tuesday June 4th 2024 3:15:10 pm
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
#pragma once

#include <compiler.h>

typedef uint8_t byte;
namespace smu
{
	/**
	 * @brief bit fields: 0 = read | 1 = write | 2 = execute | 3-7: reserved MBZ
	 * 
	 */
	enum class permissions {
		readonly		=0b00000001,
		writeonly		=0b00000010,
		execute			=0b00000100,
		readWrite		=0b00000011,
		readExecute		=0b00000101,
		writeExecute	=0b00000110,
		readWriteExecute=0b00000111
	};

	class section {
	protected: 
		/**
		 * @brief stores the sections name
		 */
		std::string name;
		/**
		 * @brief stores where this section needs to go when the compiler is generating output
		 */
		std::string targetFile;
		/**
		 * @brief stores the sections size inside the file
		 */
		uint64_t sizeInFile;
		/**
		 * @brief stores the sections size when loaded to memory
		 */
		uint64_t sizeInMemory;
		/**
		 * @brief stores the data
		 */
		byte* data;
		bool readable;
		bool writeable;
		bool executable;
		/**
		 * @brief load aligment, refer to ELF64 p_align for details (https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)
		 */
		uint64_t loadAlignment;
		/**
		 * @brief specifies the virtual address at which this section is to be loaded, 0 lets the output logic choose an address
		 */
		uint64_t virtualAddress;
	public:
		section();
		section(std::string name,std::string targetFile,permissions perms, uint64_t loadAlignment = 0, uint64_t virtualAddress = 0);
		/**
		 * @brief adds all bytes in the initializer list to the end of the sections current data
		 * 
		 * @param data
		 */
		void operator<<(std::initializer_list<byte> data);
		/**
		 * @brief adds padding bytes until this->size reaches a multiple of n
		 * 
		 * @param n 
		 * @return uint64_t the new value of this->size
		 */
		uint64_t align(uint64_t n);
		/**
		 * @brief returns the amount of bytes that are to be loaded to memory for this section
		 * 
		 * @return uint64_t
		 */
		uint64_t size();
	};
}