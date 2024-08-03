/*
 * Created Date: Tuesday June 4th 2024
 * Author: Lilith
 * -----
 * Last Modified: Sat Aug 03 2024
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

#include <SMU.h>
#include <issues.hxx>
using namespace issues;

section* code = nullptr;

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
		if(this->data != nullptr)
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

	void section::operator<<(section* data)
	{
		uint64_t newSize = this->sizeInFile+data->sizeInFile;
		this->data = (byte*)realloc(this->data,newSize);
		uint64_t I = this->sizeInFile;
		for(uint64_t II = 0;II<data->sizeInFile;II++)
			this->data[I++] = data->data[II];
		//,
		//, add and adjust relocations
		//,
		for(smu::RelocationEntry& re : data->Relocations){
			this->Relocations.push_back(
				smu::RelocationEntry(
					re.offset+this->sizeInFile,
					re.size,
					re.type,
					re.symbol
				)
			);
		}
		//,
		//, add and adjust symbols
		//,
		for(cgu::Symbol& s : data->symbols)
		{
			this->symbols.push_back(s);
			if((((uint64_t)s.Type) & 0x0F) != 0x03)
				this->symbols.back().value += this->size();
		}
		this->sizeInFile = newSize;
	}

	void section::placeSymbol(SymbolType Type, uint64_t size,std::string name)
	{
		if((((uint64_t)Type) & 0x0F) == 0x03)
			compilerBug("section::placeSymbol called with Type == (Local|Global|External)Value");
		this->symbols.push_back(Symbol(Type,this->size(),size,name));
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

	uint64_t section::size()
	{
		return this->sizeInFile > this->sizeInMemory ? this->sizeInFile : this->sizeInMemory;
	}

	uint64_t section::align(uint64_t n)
	{
		uint64_t bytesNeeded = n - (this->sizeInFile % n);
		if(bytesNeeded == 0)
			return this->sizeInFile;
		uint64_t newSize = this->sizeInFile+bytesNeeded;
		this->data = (byte*)realloc(this->data,newSize);
		memset(this->data+this->sizeInFile,0x90,bytesNeeded);
		this->sizeInFile = newSize;
		return newSize;
	}
	/**
	 * @brief adds an instruction to the data
	 * 
	 * @param Instruction a list of instruction components, added to the data in order. This also takes care of creating relocation entries
	 */
	void section::push(std::vector<InstructionComponent> Instruction) {
		for(InstructionComponent& c : Instruction) {
			switch(c.Type) {
				case(InstructionComponentType::Byte): {
					this->push(c.b);
					break;
				}
				case(InstructionComponentType::Immediate): {
					switch(c.ImmediateSize) {
						case(1): {
							this->push((u8)(c.Immediate.imm64));
							break;
						}
						case(2): {
							this->push((u16)(c.Immediate.imm64));
							break;
						}
						case(4): {
							this->push((u32)(c.Immediate.imm64));
							break;
						}
						case(8): {
							this->push((u64)(c.Immediate.imm64));
							break;
						}
					}
					if(c.Immediate.isSymbol) {
						this->Relocations.push_back(RelocationEntry(
							this->size(),
							c.ImmediateSize,
							c.ImmediateRelocType,
							c.Immediate.symbol
						));
					}
					break;
				}
				case(InstructionComponentType::Relocation): {
					c.Reloc.offset += this->size();
					for(u64 i = 0;i<c.Reloc.size;i++)this->push({0x00});
					this->Relocations.push_back(c.Reloc);
					break;
				}
				default: {
				}
			}
		}
	}
}
