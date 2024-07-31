#include "../parser.hxx"
#include <SYS_LINUX.h>
#include <output.hxx>

void parse::Keywords::Async()
{
	ParserState.Token = ParserState.Line.nextToken();
	if(ParserState.Token.type != 30)
		unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{30});
	std::vector<variable*> inputs;
	std::vector<type*> tinputs;
	uint64_t stackOffset = currentScope->func->stack->push(0);
	uint64_t dataSize = 0;
	uint64_t toff = 0;
	while(true)
	{
		ParserState.Token = ParserState.Line.nextToken();
		if(ParserState.Token.type == 31)
		{
			// end input collection
			break;
		}
		if(ParserState.Token.type == 0)
			unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{31,9});
		//resolve input variable
		line ivLine = ParserState.Line;
		{
			ivLine.tpos = 0;
			ivLine.text = ParserState.Token.text;
		}
		//dump("resolving",&ivLine,"");
		token ivt = ivLine.nextToken();
		variable* src = resolve(ivt);
		//,
		//, move inputs to stack
		//,
		{
			toff = currentScope->func->stack->push(src->dataType->size);
			dataSize += src->dataType->size;
			variable dst;
			amd64::VariableStorage* storage = new amd64::VariableStorage;
			dst.storage = (void*)storage;
			storage->mode = amd64::StorageMode::IndirectRegister;
			storage->reg = amd64::Register::rbp;
			storage->displacement = ImmediateValue(toff);
		}
		ParserState.Token = ParserState.Line.nextToken();
		if(ParserState.Token.type == 31)
		{
			// end input collection
			break;
		}
		if(ParserState.Token.type == 42)
		{
			continue;
		}
	}
	//listdump("async inputs",&inputs,"");
	std::string threadCodeSymbol = getNewName();
	//,
	//, create new stack (result stored in rax)
	//,
	{
		//+
		//+ Linux
		//+
		if(currentSystem == System::Linux)
		{
			//+
			//+ mmap system call
			//+
			{
				//. clear rdi, let system choose address
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::_xor::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::rdi,amd64::Register::rdi)
				});
				//. load 0x1000 to rsi, size = 4KiB
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__imm16_32_64(amd64::Register::rsi)
				});
				code->push(amd64::imm64(0x1000));
				//. load flags to rdx
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__imm16_32_64(amd64::Register::rdx)
				});
				code->push(amd64::imm64((1<<0) | (1<<1)));
				//. load flags to r10
				code->push({
					amd64::prefix::REX(1,0,0,1),
					amd64::opcode::mov::r16_32_64__imm16_32_64(amd64::Register::r10)
				});
				code->push(amd64::imm64((1<<1) | (1<<5) | (1<<8)));
				//. clear r8
				code->push({
					amd64::prefix::REX(1,1,0,1),
					amd64::opcode::_xor::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::r8,amd64::Register::r8)
				});
				//. clear r9
				code->push({
					amd64::prefix::REX(1,1,0,1),
					amd64::opcode::_xor::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::r9,amd64::Register::r9)
				});
				//. load sys_mmap (9) to rax
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__imm16_32_64(amd64::Register::rax)
				});
				code->push(amd64::imm64(9));
				//. fire system call
				code->push({
					0x0F,
					amd64::opcode::secondary::syscall
				});
			}
			//+
			//+ check for mmap errors
			//+
			{
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::test::rm16_32_64__r16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::Register::rax)
				});
				code->push({
					amd64::opcode::jcc::rel8off(amd64::Condition::NotSign),
					27
				});
				//. load 0x8000000000001000 to rdi
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__imm16_32_64(amd64::Register::rdi)
				});
				code->push(amd64::imm64(0x8000000000001000));
				//. compare rax to rdi
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::cmp::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::Register::rdi)
				});
				//. jump if rax >= rdi
				code->push({
					amd64::opcode::jcc::rel8off(amd64::Condition::GreaterOrEqual),
					12
				});
				//. load error handler address and call
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__imm16_32_64(amd64::Register::rdi)
				});
				code->push(amd64::imm64(0));
				code->push({
					amd64::opcode::call::rm16_32_64,
					amd64::modRM(2,amd64::AddressingMode::RegisterDirect,amd64::Register::rax)
				});
			}
		}
	}
	//,
	//, create copy of input data
	//,
	{
		uint64_t bytes = dataSize;
		uint64_t disp = 0;
		uint64_t mbdisp = toff;
		::amd64::AddressingMode addrMode = (bytes+mbdisp) <= (0b01111111) ? ::amd64::AddressingMode::RegisterIndirect_disp8 : ::amd64::AddressingMode::RegisterIndirect_disp32;
		uint64_t opsize = 8;
		amd64::Register src_base = amd64::Register::rbp;
		amd64::Register dst_base = amd64::Register::rax;
		while(opsize > 0)
		{
			while(bytes >= opsize)
			{
				//,
				//, copy data to rdx
				//,
				{
					if(opsize == 2)code->push({::amd64::prefix::legacy::OperandSizeOverride});
					code->push({
						::amd64::prefix::REX((opsize == 8),((src_base & (1<<4))>>4),0,((dst_base & (1<<4))>>4)),
						(opsize > 1 ? ::amd64::opcode::mov::r16_32_64__rm16_32_64 : ::amd64::opcode::mov::r8__rm8),
						::amd64::modRM(::amd64::Register::rax,addrMode,src_base)
					});
					switch(addrMode)
					{
						case(::amd64::AddressingMode::RegisterIndirect_disp8):{
							byte b = (byte)disp+mbdisp;
							code->push({b});
							break;
						}case(::amd64::AddressingMode::RegisterIndirect_disp32):
							code->push(::amd64::imm32(disp+mbdisp));
							break;
						default:
							compilerBug("this code is supposed to be unreachable.");
					}	
				}
				//,
				//, copy data from rax to dst
				//,
				{
					if(opsize == 2)code->push({::amd64::prefix::legacy::OperandSizeOverride});
					code->push({
						::amd64::prefix::REX((opsize == 8),((dst_base & (1<<4))>>4),0,((dst_base & (1<<4))>>4)),
						(opsize > 1 ? ::amd64::opcode::mov::rm16_32_64__r16_32_64 : ::amd64::opcode::mov::rm8__r8),
						::amd64::modRM(::amd64::Register::rax,addrMode,dst_base)
					});
					switch(addrMode)
					{
						case(::amd64::AddressingMode::RegisterIndirect_disp8):
							code->push({(byte)disp});
							break;
						case(::amd64::AddressingMode::RegisterIndirect_disp32):
							code->push(::amd64::imm32(disp));
							break;
						default:
							compilerBug("this code is supposed to be unreachable.");
					}
				}
				disp+=opsize;
				bytes-=opsize;
			}
			opsize/=2;
		}
	}
	//,
	//, call system
	//,
	{
		if(currentSystem == System::Linux)
		{
			//+
			//+ sys_clone
			//+
			{
				using namespace linux_6;
				uint64_t clone_flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_THREAD | CLONE_PTRACE;
				//. load flags to rdi
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__imm16_32_64(amd64::Register::rdi)
				});
				code->push(amd64::imm64(clone_flags));
				//. copy new stack pointer from rax to rsi
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::rsi,amd64::Register::rax)
				});
				//. load sys_clone (56) to rax
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__imm16_32_64(amd64::Register::rax)
				});
				code->push(amd64::imm64(56));
				//. fire system call
				code->push({
					0x0F,
					amd64::opcode::secondary::syscall
				});
			}
			code->push({
				amd64::prefix::REX(1,0,0,0),
				amd64::opcode::test::rm16_32_64__r16_32_64,
				amd64::modRM(amd64::Register::rax,amd64::Register::rax)
			});
			code->push({
				amd64::opcode::jcc::rel8off(amd64::Condition::NotZero),
				18
			});
		}
		//,
		//, new thread code
		//,
		{
			code->push({
				amd64::prefix::REX(1,0,0,0),
				amd64::opcode::mov::r16_32_64__rm16_32_64,
				amd64::modRM(amd64::Register::rbp,amd64::Register::rsp)
			});
			code->push({
				amd64::opcode::jmp::rel16_32,
				0,0,0,0
			});
			//+
			//+ linker info
			//+
			{
				code->Relocations.push_back(
					smu::RelocationEntry(
						code->size()-4,
						4,
						smu::RelocationType::Relative,
						threadCodeSymbol
					)
				);
			}
		}
		//,
		//, old thread code
		//,
		{
		}
	}
	//,
	//, create scope
	//,
	{
		scope* sc = new scope;
		function* func = new function;
		func->__declared_file = ParserState.File;
		func->__declared_line = ParserState.Line.lineNum;
		func->name = threadCodeSymbol;
		func->parameters = tinputs;
		func->vparams = inputs;
		func->returnType = defaultUnsignedIntegerType;
		func->code = new section;
		func->isDeprecated = false;
		func->isPrimitive = false;
		func->primitiveFloat = false;
		func->primitiveInPlace = false;
		func->op = primitiveOP::invalid;
		func->noReturn = true;
		func->noDoc = true;
		func->doExport = false;
		func->abi = currentScope->func->abi;
		func->symbol = threadCodeSymbol;
		func->isLocal = false;
		func->miscData = (1<<1);
		sc->name = func->name;
		sc->func = func;
		sc->leadingSpace = currentScope->leadingSpace+tabLength;
		sc->parent = currentScope;
		sc->t = scopeType::FUNCTION;
		ParserState.Token = ParserState.Line.nextToken();
		for(variable* i : inputs)
			sc->variables.push_back(i);
		if(ParserState.Token.type == 36)
			sc->isIndentBased = false;
		else if(ParserState.Token.type == 40)
			sc->isIndentBased = true;
		else
			unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{36,40});
		Entity::updateCurrentScope(sc);
	}
	struct RoutineData_T {
		section* BodyCode;
		scope* sc;
		function* func;
	};
	RoutineData_T* RoutineData = new RoutineData_T;{
		RoutineData->BodyCode = currentScope->func->code;
		RoutineData->sc = currentScope;
		RoutineData->func = currentScope->func;
	}
	currentScope->StartClosure.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code = new section;
		}
	));
	currentScope->Prologue.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
		}
	));
	currentScope->BodyCode.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code->push(data->BodyCode);
		}
	));
	currentScope->Epilogue.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			variable v_null;
			v_null.storageArch = Architecture::storage_IntegerImmediate;
			v_null.storage = (void*)0;
			v_null.dataType = defaultUnsignedIntegerType;
			std::vector<variable*> args = {&v_null};
			function* fexit = getFunction("exit",args);
			std::cout << "fexit: " << fexit->expression_ansi() << std::endl;
			call(fexit,args);
		}
	));
	currentScope->Finalize.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			for(Routine& r : data->sc->BranchCode)
				r.run();
			text.placeSymbol(SymbolType::LocalFunction,code->size(),data->sc->name);
			text.push(code);
		}
	));
	currentScope->Destroy.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			delete code;
		}
	));
}
