#pragma once

#include <compiler.h>

#include <cgu.h>

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

	enum class RelocationType : uint8_t {
		Invalid 	= 0x00,
		Absolute 	= 0x01,
		Relative	= 0x02,
	};

	class RelocationEntry {
	public:
		uint64_t offset;
		uint8_t size;
		RelocationType type;
		std::string symbol;
		RelocationEntry(){}
		RelocationEntry(uint64_t offset, uint8_t size, RelocationType type, std::string symbol)
			:offset(offset),size(size),type(type),symbol(symbol){}
	};

	enum class InstructionComponentType {
		Invalid = 0,
		Relocation,
		Immediate,
		Byte
	};
	class InstructionComponent {
	public:
		InstructionComponentType Type = InstructionComponentType::Invalid;
		RelocationEntry Reloc;
		ImmediateValue Immediate;
		RelocationType ImmediateRelocType;
		u8 ImmediateSize;
		byte b;
		InstructionComponent(ImmediateValue Immediate,u8 size,RelocationType Type)
			:Immediate(Immediate),ImmediateSize(size),ImmediateRelocType(Type),Type(InstructionComponentType::Immediate){}
		InstructionComponent(RelocationEntry reloc)
			:Reloc(reloc),Type(InstructionComponentType::Relocation){}
		InstructionComponent(byte b)
			:b(b),Type(InstructionComponentType::Byte){}
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
		uint64_t sizeInFile = 0;
		/**
		 * @brief stores the sections size when loaded to memory
		 */
		uint64_t sizeInMemory = 0;
	public:
		std::vector<RelocationEntry> Relocations;
		std::vector<Symbol> symbols;
		/**
		 * @brief stores the data
		 */
		byte* data = nullptr;
	protected:
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
		~section();
		section(std::string name,std::string targetFile,permissions perms, uint64_t loadAlignment = 0, uint64_t virtualAddress = 0);
		/**
		 * @brief adds all bytes in the initializer list to the end of the sections current data
		 * 
		 * @param data
		 */
		void operator<<(std::initializer_list<byte> data);
		/**
		 * @brief adds all the bytes from uint64_t data to current data in the order in which they currently appear in memory
		 * 
		 * @param data
		 */
		void operator<<(uint64_t data);
		/**
		 * @brief adds all the bytes from uint32_t data to current data in the order in which they currently appear in memory
		 * 
		 * @param data
		 */
		void operator<<(uint32_t data);
		/**
		 * @brief adds all the bytes from uint16_t data to current data in the order in which they currently appear in memory
		 * 
		 * @param data
		 */
		void operator<<(uint16_t data);
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
		/**
		 * @brief adds all the data from section& data to this section
		 * 
		 * @param data 
		 */
		void operator<<(section* data);
		/**
		 * @brief adds all the data from section& data to this section
		 * 
		 * @param data 
		 */
		void push(section* data){*this << data;}
		/**
		 * @brief adds all bytes in the initializer list to the end of the sections current data
		 * 
		 * @param data
		 */
		void push(std::initializer_list<byte> data){*this << data;}
		/**
		 * @brief adds data[0 through (n-1)] to the current data
		 * 
		 */
		template<typename T>
		void push(T* data, uint64_t n)
		{
			uint64_t newSize = this->sizeInFile+(n*sizeof(T));
			this->data = (byte*)realloc(this->data,newSize);
			memcpy(this->data+this->sizeInFile,data,(n*sizeof(T)));
			this->sizeInFile = newSize;
		}
		/**
		 * @brief adds an instruction to the data
		 * 
		 * @param Instruction a list of instruction components, added to the data in order this also takes care of creating relocation entries
		 */
		void push(std::vector<InstructionComponent> Instruction);
		/**
		 * @brief adds data to the current data
		 * 
		 */
		template<typename T>
		void push(T data)
		{
			uint64_t newSize = this->sizeInFile+sizeof(data);
			this->data = (byte*)realloc(this->data,newSize);
			memcpy(this->data+this->sizeInFile,&data,sizeof(data));
			this->sizeInFile = newSize;
		}
		/**
		 * @brief adds a symbols at sections current end
		 * 
		 * @param Type 
		 * @param size 
		 * @param name 
		 */
		void placeSymbol(SymbolType Type, uint64_t size,std::string name);
	};
}

using smu::section;

extern section* code;
