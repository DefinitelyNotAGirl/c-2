/*
 * Created Date: Tuesday June 4th 2024
 * Author: Lilith
 * -----
 * Last Modified: Tuesday June 4th 2024 4:55:46 pm
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

#include <SMU.h>

namespace smu
{
	section::section(){}
	section::section(std::string name,std::string targetFile,permissions perms, uint64_t loadAlignment, uint64_t virtualAddress)
		:name(name),targetFile(targetFile),loadAlignment(loadAlignment),virtualAddress(virtualAddress)
	{
		this->readable 	= ((((uint64_t)perms) & (1<<0)) >> 0);
		this->writeable = ((((uint64_t)perms) & (1<<1)) >> 1);
		this->executable= ((((uint64_t)perms) & (1<<2)) >> 2);
	}
	
	section::~section() {
		free(this->data);
	}

	void section::operator<<(std::initializer_list<byte> data)
	{
		uint64_t newSize = this->sizeInFile+data.size();
		this->data = (byte*)realloc(this->data,newSize);
		uint64_t I = this->sizeInFile;
		for(byte II : data)
			this->data[I++] = II;
		this->sizeInFile = newSize;
	}

	void section::operator<<(std::initializer_list<section> data)
	{
		uint64_t newSize = this->sizeInFile;
		for(section& s : data)
			newSize+=s.sizeInFile;
		this->data = (byte*)realloc(this->data,newSize);
		uint64_t I = this->sizeInFile;
		for(section& II : data)
			for(uint64_t III = 0;III<II.sizeInFile;III++)
				this->data[I++] = II.data[III];
		this->sizeInFile = newSize;
	}

	void section::operator<<(section& data)
	{
		uint64_t newSize = this->sizeInFile+data.sizeInFile;
		this->data = (byte*)realloc(this->data,newSize);
		uint64_t I = this->sizeInFile;
		for(uint64_t II = 0;II<data.sizeInFile;II++)
			this->data[I++] = data.data[II];
		this->sizeInFile = newSize;
	}

	void section::operator<<(uint64_t data)
	{
		uint64_t newSize = this->sizeInFile+sizeof(data);
		this->data = (byte*)realloc(this->data,newSize);
		memcpy(this->data+this->sizeInFile,&data,sizeof(data));
		this->sizeInFile = newSize;
	}

	void section::operator<<(uint32_t data)
	{
		uint64_t newSize = this->sizeInFile+sizeof(data);
		this->data = (byte*)realloc(this->data,newSize);
		memcpy(this->data+this->sizeInFile,&data,sizeof(data));
		this->sizeInFile = newSize;
	}

	void section::operator<<(uint16_t data)
	{
		uint64_t newSize = this->sizeInFile+sizeof(data);
		this->data = (byte*)realloc(this->data,newSize);
		memcpy(this->data+this->sizeInFile,&data,sizeof(data));
		this->sizeInFile = newSize;
	}

	void section::size()
	{
		return this->sizeInFile > this->sizeInMemory ? this->sizeInFile : this->sizeInMemory;
	}

	void section::align(uint64_t n)
	{
		uint64_t bytesNeeded = n - (this->sizeInFile % n);
		if(bytesNeeded == 0)
			return;
		uint64_t newSize = this->sizeInFile+bytesNeeded;
		this->data = (byte*)realloc(this->data,newSize);
		memset(this->data+this->sizeInFile,0x90,bytesNeeded);
		this->sizeInFile = newSize;
	}
}
