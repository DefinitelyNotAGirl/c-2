/*
 * Created Date: Thursday June 6th 2024
 * Author: Lilith
 * -----
 * Last Modified: Thursday June 6th 2024 10:09:55 am
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

#include <amd64.cgu.hxx>
#include <CGU.AMD64.h>
#include <SMU.h>
#include <issues.hxx>

using namespace issues;

namespace amd64
{
	uint64_t imm64(uint64_t value)
	{
		return value;
	}

	/**
	 * @brief this is short hand for modRM(reg,AddressingMode::RegisterDirect,rm)
	 * 
	 * @param reg reg operrand
	 * @param rm reg/mem opperand
	 * @return amd64 modRM byte ready for execution
	 *
	 */
	byte modRM(Register reg,Register rm)
	{
		return modRM(reg,AddressingMode::RegisterDirect,rm);
	}
	/**
	 * @brief
	 * 
	 * @param reg reg operrand
	 * @param rm reg/mem opperand
	 * @return amd64 modRM byte ready for execution
	 */
	byte modRM(Register reg,AddressingMode mod,Register rm)
	{
		return (reg<<3) | (mod<<6) | (rm<<0);
	}
	/**
	 * @brief
	 * 
	 * @param scale must be either 1,2,4 or 8 
	 * @param index register holding the index
	 * @param base register holding the base address
	 * @return amd64 SIB byte ready for execution
	 */
	byte SIB(uint8_t scale, Register index, Register base)
	{
		return (scale<<6) | (index<<3) | (base<<0);
	}

	namespace prefix
	{
		byte REX(bool W, bool R, bool X, bool B)
		{
			return 0x40 | (W<<3) | (R<<2) | (X<<1) | (B<<0);
		}
		namespace legacy
		{
		}
	}

	namespace opcode {
	}
}

namespace runtime::amd64
{
	void copy(variable* src, variable* dst)
	{
		if(src->storageArch != Architecture::AMD64)
			compilerBug("invalid architecture",originCoreHere,source(),"");
		if(dst->storageArch != Architecture::AMD64)
			compilerBug("invalid architecture",originCoreHere,source(),"");
		::amd64::VariableStorage* srcStore = src->storage;
		::amd64::VariableStorage* dstStore = dst->storage;
//,####################################################################################################################
//,####################################################################################################################
//, ██████  ██████                       ██       ██████  ██████
//, ██   ██ ██   ██                       ██      ██   ██ ██   ██
//, ██   ██ ██████      █████ █████ █████  ██     ██   ██ ██████
//, ██   ██ ██   ██                       ██      ██   ██ ██   ██
//, ██████  ██   ██                      ██       ██████  ██   ██
//,####################################################################################################################
//,####################################################################################################################
		if( 
			(srcStore->mode == ::amd64::StorageMode::DirectRegister)
			&&
			(dstStore->mode == ::amd64::StorageMode::DirectRegister)
		)
		{
			code->push({
				::amd64::prefix::REX(0,((srcStore->reg & (1<<4))>>4),0,((dstStore->reg & (1<<4))>>4)),
				::amd64::opcode::mov::r16_32_64__rm16_32_64,
				::amd64::modRM(srcStore->reg,dstStore->reg)
			});
		}
	}
	void clear(variable* target);
	void thread(std::string& symbol);
	void call(function* func);
	void enter(uint64_t frameSize);
	void leave();
}
