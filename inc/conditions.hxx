#include <extint.hxx>
#include <stack>

#ifndef global
	#define global extern
#endif

enum class Condition : u8 {
	/* compile time condition codes */
	False = 0xF0,
	True  = 0xF1,
	/* amd64 condition codes*/
	Overflow 		= 0x0,
	NotOverflow		= 0x1,
	Below			= 0x2,
	Carry			= 0x2,
	NotAboveOrEqual	= 0x2,
	NotBelow		= 0x3,
	NotCarry		= 0x3,
	AboveOrEqual	= 0x3,
	Zero			= 0x4,
	Equal			= 0x4,
	NotZero			= 0x5,
	NotEqual		= 0x5,
	BelowOrEqual	= 0x6,
	NotAbove		= 0x6,
	NotBelowOrEqual	= 0x7,
	Above			= 0x7,
	Sign			= 0x8,
	NotSign			= 0x9,
	Parity			= 0xA,
	ParityEven		= 0xA,
	NotParity		= 0xB,
	ParityOdd		= 0xB,
	Less			= 0xC,
	NotGreaterOrEqual=0xC,
	NotLess			= 0xD,
	GreaterOrEqual	= 0xD,
	LessOrEqual		= 0xE,
	NotGreater		= 0xE,
	NotLessOrEqual	= 0xF,
	Greater			= 0xF,
};
global std::stack<Condition> ConditionCode;
