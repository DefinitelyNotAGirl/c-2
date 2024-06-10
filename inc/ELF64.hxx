/*
 * Created Date: Monday June 10th 2024
 * Author: Lilith
 * -----
 * Last Modified: Monday June 10th 2024 4:03:49 am
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

namespace elf64
{
	#pragma pack(1)
	constexpr uint32_t Magic = 0x464c457f;
	struct SectionHeader
	{
	    uint32_t Name;
	    uint32_t Type;
	    uint64_t Flags;
	    uint64_t VirtualAddress;
	    uint64_t OffsetInFile;
	    uint64_t SizeInFile;
	    uint32_t Link;
	    uint32_t Info;
	    uint64_t Alignment;
	    uint64_t EntrySize;
	};

	struct ProgramHeader
	{
    	uint32_t Type;
    	uint32_t Flags;
    	uint64_t OffsetInFile;
    	uint64_t VirtualAddress;
    	uint64_t PhysicalAddress;
    	uint64_t SizeInFile;
    	uint64_t SizeInMemory;
    	uint64_t Alignment;
	};

	struct Header
	{
    	uint32_t Magic;
    	uint8_t Class;
    	uint8_t Endianess;
    	uint8_t ElfVersion;
    	uint8_t SystemABI;
    	uint8_t AbiVersion;
    	uint8_t padding0[7];
    	uint16_t Type;
    	uint16_t Machine;
    	uint32_t Version;
    	uint64_t EntryPoint;
    	uint64_t ProgramHeaderTableOffset;
    	uint64_t SectionHeaderTableOffset;
    	uint32_t Flags;
    	uint16_t Length;
    	uint16_t ProgramHeaderSize;
    	uint16_t ProgramHeaderCount;
    	uint16_t SectionHeaderSize;
    	uint16_t SectionHeaderCount;
    	uint16_t SectionNameEntry;
	};

	namespace shf
	{
		constexpr uint64_t write 			= (1<< 0);
		constexpr uint64_t alloc 			= (1<< 1);
		constexpr uint64_t execinstr 		= (1<< 2);
		constexpr uint64_t merge			= (1<< 4);
		constexpr uint64_t strings			= (1<< 5);
		constexpr uint64_t info_link		= (1<< 6);
		constexpr uint64_t link_order		= (1<< 7);
		constexpr uint64_t os_nonconforming	= (1<< 8);
		constexpr uint64_t group			= (1<< 9);
		constexpr uint64_t tls				= (1<<10);
		constexpr uint64_t ordered 			= 0x4000000;
		constexpr uint64_t exclude 			= 0x8000000;
		constexpr uint64_t mask_os			= 0x0FF00000;
		constexpr uint64_t mask_proc		= 0xF0000000;
	}
	#pragma pack()
}