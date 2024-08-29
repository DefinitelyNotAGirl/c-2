#pragma once

#include <cstdint>
#include <cstring>
#include "cmath"

#define uint64_t_max 0xFFFFFFFFFFFFFFFF

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

inline uint64_t negative(uint64_t n){return (n | ((uint64_t)1<<63));}
inline uint64_t positive(uint64_t n){return (n & (~((uint64_t)1<<63)));}
template<typename src,typename dst>
inline dst copySignBit(src n)
{
	src sign = (n &  (1<<((sizeof(src)/8)-2)));
	dst val  = (n & ~(1<<((sizeof(src)/8)-1)));
	return (val | sign);
}

typedef uint8_t byte;

class zero{};

inline uint64_t extintCast(zero*){return 0;}

class uint80_t
{
private:
public:
	constexpr const static uint64_t byteWidth = 10;
	constexpr const static uint64_t bitWidth  = 80;
	byte data[byteWidth];
	uint80_t(){}
	uint80_t(zero*,zero*)
	{
		(*((uint64_t*)(this->data+0))) = 0;
		
		(*((uint16_t*)(this->data+8))) = 0;
	}

	template<typename T>
	uint80_t(T n)
	{
		if(this->byteWidth <= T::byteWidth)
			memcpy(this->data,n.data,this->byteWidth);
		else
			memcpy(this->data,n.data,T::byteWidth);
	}

	void operator=(byte* data)
	{
		memcpy(this->data,data,this->byteWidth);
	}
};

class uint128_t
{
private:
public:
	constexpr const static uint64_t byteWidth =  16;
	constexpr const static uint64_t bitWidth  = 128;
	byte data[byteWidth];
	uint128_t(){}
	uint128_t(zero*,zero*)
	{
		(*((uint64_t*)(this->data+0))) = 0;
		(*((uint64_t*)(this->data+8))) = 0;
	}

	template<typename T>
	uint128_t(T n)
	{
		if(this->byteWidth <= T::byteWidth)
			memcpy(this->data,n.data,this->byteWidth);
		else
			memcpy(this->data,n.data,T::byteWidth);
	}

	void operator=(byte* data)
	{
		memcpy(this->data,data,this->byteWidth);
	}
};

class uint256_t
{
public:
	constexpr const static uint64_t byteWidth =  32;
	constexpr const static uint64_t bitWidth  = 256;
	byte data[byteWidth];
	uint256_t(){}
	uint256_t(zero*,zero*)
	{
		(*((uint64_t*)(this->data+ 0))) = 0;
		(*((uint64_t*)(this->data+ 8))) = 0;
		(*((uint64_t*)(this->data+16))) = 0;
		(*((uint64_t*)(this->data+24))) = 0;
	}

	template<typename T>
	uint256_t(T n)
	{
		if(this->byteWidth <= T::byteWidth)
			memcpy(this->data,n.data,this->byteWidth);
		else
			memcpy(this->data,n.data,T::byteWidth);
	}

	void operator=(byte* data)
	{
		memcpy(this->data,data,this->byteWidth);
	}
};

inline uint256_t extintCast(uint256_t*,uint256_t n){return n;}
inline uint256_t extintCast(uint256_t*,uint128_t n){return uint256_t(n);}
inline uint256_t extintCast(uint256_t*, uint80_t n){return uint256_t(n);}
inline uint256_t extintCast(uint256_t*, uint64_t n){return uint256_t((zero*)0,(zero*)0);}
inline uint256_t extintCast(uint256_t*, uint32_t n){return uint256_t((zero*)0,(zero*)0);}
inline uint256_t extintCast(uint256_t*, uint16_t n){return uint256_t((zero*)0,(zero*)0);}
inline uint256_t extintCast(uint256_t*,  uint8_t n){return uint256_t((zero*)0,(zero*)0);}

inline uint128_t extintCast(uint128_t*,uint256_t n){return uint128_t(n);}
inline uint128_t extintCast(uint128_t*,uint128_t n){return n;}
inline uint128_t extintCast(uint128_t*, uint80_t n){return uint128_t(n);}
inline uint128_t extintCast(uint128_t*, uint64_t n){return uint128_t((zero*)0,(zero*)0);}
inline uint128_t extintCast(uint128_t*, uint32_t n){return uint128_t((zero*)0,(zero*)0);}
inline uint128_t extintCast(uint128_t*, uint16_t n){return uint128_t((zero*)0,(zero*)0);}
inline uint128_t extintCast(uint128_t*,  uint8_t n){return uint128_t((zero*)0,(zero*)0);}

inline  uint80_t extintCast( uint80_t*,uint256_t n){return uint80_t(n);}
inline  uint80_t extintCast( uint80_t*,uint128_t n){return uint80_t(n);}
inline  uint80_t extintCast( uint80_t*, uint80_t n){return n;}
inline  uint80_t extintCast( uint80_t*, uint64_t n){return uint80_t((zero*)0,(zero*)0);}
inline  uint80_t extintCast( uint80_t*, uint32_t n){return uint80_t((zero*)0,(zero*)0);}
inline  uint80_t extintCast( uint80_t*, uint16_t n){return uint80_t((zero*)0,(zero*)0);}
inline  uint80_t extintCast( uint80_t*,  uint8_t n){return uint80_t((zero*)0,(zero*)0);}

inline  uint64_t extintCast( uint64_t*,uint256_t n){return 0;}
inline  uint64_t extintCast( uint64_t*,uint128_t n){return 0;}
inline  uint64_t extintCast( uint64_t*, uint80_t n){return 0;}
inline  uint64_t extintCast( uint64_t*, uint64_t n){return n;}
inline  uint64_t extintCast( uint64_t*, uint32_t n){return n;}
inline  uint64_t extintCast( uint64_t*, uint16_t n){return n;}
inline  uint64_t extintCast( uint64_t*,  uint8_t n){return n;}

inline  uint32_t extintCast( uint32_t*,uint256_t n){return 0;}
inline  uint32_t extintCast( uint32_t*,uint128_t n){return 0;}
inline  uint32_t extintCast( uint32_t*, uint80_t n){return 0;}
inline  uint32_t extintCast( uint32_t*, uint64_t n){return n;}
inline  uint32_t extintCast( uint32_t*, uint32_t n){return n;}
inline  uint32_t extintCast( uint32_t*, uint16_t n){return n;}
inline  uint32_t extintCast( uint32_t*,  uint8_t n){return n;}

inline  uint16_t extintCast( uint16_t*,uint256_t n){return 0;}
inline  uint16_t extintCast( uint16_t*,uint128_t n){return 0;}
inline  uint16_t extintCast( uint16_t*, uint80_t n){return 0;}
inline  uint16_t extintCast( uint16_t*, uint64_t n){return n;}
inline  uint16_t extintCast( uint16_t*, uint32_t n){return n;}
inline  uint16_t extintCast( uint16_t*, uint16_t n){return n;}
inline  uint16_t extintCast( uint16_t*,  uint8_t n){return n;}

inline   uint8_t extintCast(  uint8_t*,uint256_t n){return 0;}
inline   uint8_t extintCast(  uint8_t*,uint128_t n){return 0;}
inline   uint8_t extintCast(  uint8_t*, uint80_t n){return 0;}
inline   uint8_t extintCast(  uint8_t*, uint64_t n){return n;}
inline   uint8_t extintCast(  uint8_t*, uint32_t n){return n;}
inline   uint8_t extintCast(  uint8_t*, uint16_t n){return n;}
inline   uint8_t extintCast(  uint8_t*,  uint8_t n){return n;}

template<typename srctype, typename dsttype>
inline dsttype extintCast(srctype n){return dsttype(n);}
