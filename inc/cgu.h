#pragma once
#include <cstdint>
#include <string>
#include <vector>
//#include <class_variable.h>
//#include <function.h>
class function;
class variable;
#include <issues.hxx>
namespace cgu
{
	enum class SymbolType : uint8_t {
		Invalid 			= 0x00,
		CodeLocation 		= 0x01,
		GlobalFunction		= 0x11,
		GlobalVariable		= 0x12,
		GlobalValue			= 0x13,
		LocalFunction 		= 0x21,
		LocalVariable 		= 0x22,
		LocalValue	 		= 0x23,
		ExternalFunction	= 0x31,
		ExternalVariable	= 0x32,
		ExternalValue		= 0x33,
	};

	class Symbol {
	public:
		SymbolType Type;
		uint64_t value;
		uint64_t size;
		std::string name;
		Symbol(SymbolType Type,uint64_t value, uint64_t size,std::string name)
		:Type(Type),value(value),size(size),name(name) {
			if((((uint64_t)Type) & 0x0F) == 0x03)
				issues::compilerBug("main symbol constructor invoked for value type symbol");
		}
		Symbol(SymbolType Type,uint64_t value,std::string name)
		:Type(Type),value(value),name(name),size(0){}
	};

	extern std::vector<Symbol> symbols;
}

using cgu::Symbol;
using cgu::SymbolType;

namespace runtime
{
	/**
	 * @brief copies the data from src to dst
	 * 
	 * @param src 
	 * @param dst 
	 */
	void copy(variable* src, variable* dst);

	/**
	 * @brief clears all bytes in target to zero
	 * 
	 * @param target 
	 */
	void clear(variable* target);

	/**
	 * @brief starts a new thread which will run the code at symbol
	 * 
	 * @param symbol 
	 */
	void thread(std::string& symbol);

	/**
	 * @brief calls func, this does not pass any arguments
	 * 
	 * @param func 
	 */
	void call(function* func);

	/**
	 * @brief creates a new stack frame of size frameSize
	 * 
	 * @param frameSize
	 */
	void enter(uint64_t frameSize);

	/**
	 * @brief destroys the current stack frame
	 * 
	 */
	void leave();

	/**
	 * @brief performs unsigned integer addition
	 * 
	 * @param a first input
	 * @param b second input
	 * @return variable holding the result
	 */
	variable* UnsignedIntegerAddition(variable* a, variable* b);

	/**
	 * @brief transfer control to a relative address
	 * 
	 */
	void RelativeControlTransfer(ImmediateValue offset);

	/**
	 * @brief transfer control to an absolute address
	 * 
	 */
	void AbsoluteControlTransfer(ImmediateValue address);

	/**
	 * @brief save all user registers to the stack except the stack pointer, the frame pointer and the thread-data pointer
	 * @return stack frame offset of saved data
	 */
	uint64_t SaveAll();

	/**
	 * @brief load all user registers from the stack except the stack pointer, the frame pointer and the thread-data pointer
	 */
	void LoadAll(uint64_t offset);
}
