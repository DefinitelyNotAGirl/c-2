#include "../parser.hxx"

#include <output.hxx>

void parse::Keywords::Throw()
{
	if(options::ddebug)std::cout << "throw" << std::endl;
	if(currentScope->t == scopeType::GLOBAL)
		invalidUseOfKeywordInScope("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),currentScope);
	ParserState.Token = ParserState.Line.nextToken();
	//,
	//, get data to throw
	//,
	variable* throwVal = resolve(ParserState.Token);//throw this variable
	{
		//+
		//+ check if throwVal is held in rax
		//+
		{
			//+
			//+ check if throwVal does not have proper storage (storage_IntegerImmediate or such)
			//+
			if(throwVal->storageArch == Architecture::storage_IntegerImmediate)
			{
				if(currentArchitecture == Architecture::AMD64)
				{
					variable* src = throwVal;
					throwVal = new variable(*src);
					throwVal->storageArch = Architecture::AMD64;
					throwVal->storage = (void*)(new amd64::VariableStorage);
					((amd64::VariableStorage*)throwVal->storage)->mode = amd64::StorageMode::DirectImmediate;
					((amd64::VariableStorage*)throwVal->storage)->immediate = ImmediateValue((uint64_t)src->storage);
				}
			}
			if(throwVal->storageArch != currentArchitecture)compilerBug("architecture mismatch");
			if(currentArchitecture == Architecture::AMD64)
			{
				if(((amd64::VariableStorage*)throwVal->storage)->reg == amd64::Register::rax)
				{
					compilerBug("unimplemented: move throw data away from rax");
				}
			}
		}
	}
	std::string handlerSymbol_routine = std::string("____cpe2")+CPE2_SYMBOL_SCOPE_SEP+"exceptions"+CPE2_SYMBOL_SCOPE_SEP+throwVal->dataType->mangledName+CPE2_SYMBOL_SCOPE_SEP+"routine";
	std::string handlerSymbol_datadst = std::string("____cpe2")+CPE2_SYMBOL_SCOPE_SEP+"exceptions"+CPE2_SYMBOL_SCOPE_SEP+throwVal->dataType->mangledName+CPE2_SYMBOL_SCOPE_SEP+"datadst";						
	if(throwVal->dataType->ExceptionOffset == 0){
		importExternalValue(handlerSymbol_routine);
		importExternalValue(handlerSymbol_datadst);
	}
	if(throwVal->dataType->ExceptionOffset == 0){
		resourceCode.push_back("c2resource exception "+throwVal->dataType->mangledName+";");
	}
	throwVal->dataType->ExceptionOffset = 1;
	//,
	//, pass data and load frame pointer
	//,
	if(currentArchitecture == Architecture::AMD64)
	{
		code->push({
			amd64::prefix::REX(1,0,0,1),
			amd64::opcode::mov::r16_32_64__rm16_32_64,
			amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
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
					smu::RelocationType::Absolute,
					handlerSymbol_datadst
				)
			);
		}
		variable dst;
		dst.storageArch = Architecture::AMD64;
		dst.storage = (void*)(new amd64::VariableStorage);
		((amd64::VariableStorage*)dst.storage)->mode = amd64::StorageMode::IndirectRegister;
		((amd64::VariableStorage*)dst.storage)->displacement = ImmediateValue(0);
		((amd64::VariableStorage*)dst.storage)->reg = amd64::Register::rax;
		runtime::copy(throwVal,&dst);
		code->push({
			amd64::prefix::REX(1,0,0,0),
			amd64::opcode::mov::r16_32_64__rm16_32_64,
			amd64::modRM(amd64::Register::rbp,amd64::Register::rax)
		});
	}
	//,
	//, jump to handler
	//,
	{
		code->push({
			amd64::prefix::REX(1,0,0,1),
			amd64::opcode::mov::r16_32_64__rm16_32_64,
			amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
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
					smu::RelocationType::Absolute,
					handlerSymbol_routine
				)
			);
		}
		code->push({
			amd64::prefix::REX(1,0,0,0),
			amd64::opcode::jmp::rm16_32_64,
			amd64::modRM(amd64::Register::rax,amd64::Register::rax)
		});
	}
}
