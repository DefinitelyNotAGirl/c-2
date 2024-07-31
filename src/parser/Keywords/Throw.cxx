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
	//,
	//, pass data and load frame pointer
	//,
	if(currentArchitecture == Architecture::AMD64)
	{
		code->push({
			amd64::opcode::mov::r16_32_64__imm16_32_64(::amd64::Register::rax),
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
					throwVal->dataType->getRuntimeTypeID()
				)
			);
		}
		variable dst;
		dst.storageArch = Architecture::AMD64;
		dst.storage = (void*)(new amd64::VariableStorage);
		((amd64::VariableStorage*)dst.storage)->mode = amd64::StorageMode::DirectRegister;
		((amd64::VariableStorage*)dst.storage)->reg = amd64::Register::rdx;
		runtime::copy(throwVal,&dst);
	}
	//,
	//, jump to handler
	//,
	{
		code->push({
			::amd64::opcode::jmp::rel16_32,
			0,0,0,0
		});
		code->Relocations.push_back(
			smu::RelocationEntry(
				code->size()-4,
				4,
				smu::RelocationType::Relative,
				Entity::ExceptionHandlers.top()
			)
		);
	}
}
