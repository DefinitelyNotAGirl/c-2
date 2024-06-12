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

	enum class RelocationType : uint32_t {
		R_X86_64_NONE			= 0,	//+ No relocation
		R_X86_64_64				= 1,	//+ Direct 64-bit
		R_X86_64_PC32			= 2,	//+ PC relative 32-bit signed
		R_X86_64_GOT32			= 3,	//+ 32-bit GOT entry
		R_X86_64_PLT32			= 4,	//+ 32-bit PLT address
		R_X86_64_COPY			= 5,	//+ Copy symbol at runtime
		R_X86_64_GLOB_DAT		= 6,	//+ Create GOT entry
		R_X86_64_JUMP_SLOT		= 7,	//+ Create PLT entry
		R_X86_64_RELATIVE		= 8,	//+ Adjust by program base
		R_X86_64_GOTPCREL		= 9,	//+ 32-bit signed PC relative offset to GOT
		R_X86_64_32				= 10,	//+ Direct 32-bit zero-extended
		R_X86_64_32S			= 11,	//+ Direct 32-bit sign-extended
		R_X86_64_16				= 12,	//+ Direct 16-bit zero-extended
		R_X86_64_PC16			= 13,	//+ 16-bit sign-extended PC relative
		R_X86_64_8				= 14,	//+ Direct 8-bit sign-extended
		R_X86_64_PC8			= 15,	//+ 8-bit sign-extended PC relative
		R_X86_64_DTPMOD64		= 16,	//+ ID of module containing symbol
		R_X86_64_DTPOFF64		= 17,	//+ Offset in module's TLS block
		R_X86_64_TPOFF64		= 18,	//+ Offset in initial TLS block
		R_X86_64_TLSGD			= 19,	//+ 32-bit signed PC relative offset to two GOT entries for GD symbol
		R_X86_64_TLSLD			= 20,	//+ 32-bit signed PC relative offset to two GOT entries for LD symbol
		R_X86_64_DTPOFF32		= 21,	//+ Offset in TLS block
		R_X86_64_GOTTPOFF		= 22,	//+ 32-bit signed PC relative offset to GOT entry for IE symbol
		R_X86_64_TPOFF32		= 23,	//+ Offset in initial TLS block
		R_X86_64_PC64			= 24,	//+ PC relative 64-bit
		R_X86_64_GOTOFF64		= 25,	//+ 64-bit offset to GOT
		R_X86_64_GOTPC32		= 26,	//+ 32-bit signed PC relative offset to GOT
		R_X86_64_GOT64			= 27,	//+ 64-bit GOT entry offset
		R_X86_64_GOTPCREL64		= 28,	//+ 64-bit PC relative offset to GOT entry
		R_X86_64_GOTPC64		= 29,	//+ 64-bit PC relative offset to GOT
		R_X86_64_GOTPLT64		= 30,	//+ Like GOT64, says PLT entry needed
		R_X86_64_PLTOFF64		= 31,	//+ 64-bit GOT relative offset to PLT entry
		R_X86_64_SIZE32			= 32,	//+ Size of symbol plus 32-bit addend
		R_X86_64_SIZE64			= 33,	//+ Size of symbol plus 64-bit addend
		R_X86_64_GOTPC32_TLSDESC= 34,	//+ GOT offset for TLS descriptor.
		R_X86_64_TLSDESC_CALL	= 35,	//+ Marker for call through TLS descriptor.
		R_X86_64_TLSDESC		= 36,	//+ TLS descriptor.
		R_X86_64_IRELATIVE		= 37,	//+ Adjust indirectly by program base
		R_X86_64_RELATIVE64		= 38,	//+ 64-bit adjust by program base
		R_X86_64_GOTPCRELX		= 41,	//+ Load from 32 bit signed pc relative offset to GOT
		R_X86_64_REX_GOTPCRELX	= 42,	//+ Load from 32 bit signed pc relative offset to GOT under REX
		R_X86_64_NUM			= 43	//+ Keep this the last entry
	};

	struct RelocationEntry {
		uint64_t offset;
		uint64_t info;
		int64_t addend;
		RelocationEntry(uint64_t offset, uint32_t symbol, RelocationType rtype, int64_t addend)
			:offset(offset),info(symbol<<32 | rtype),addend(addend){}
	};
	#pragma pack()
}