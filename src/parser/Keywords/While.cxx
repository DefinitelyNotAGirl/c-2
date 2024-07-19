#include "../parser.hxx"

using namespace issues;

void parse::Keywords::While()
{
	//parse condition line
	scope* sc = new scope;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"while"+std::to_string(currentScope->whileCounter++);
	sc->parent = currentScope;
	sc->isIndentBased = true;
	sc->t = scopeType::CONDITIONAL_BLOCK;
	sc->func = new function;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	sc->reentrySymbol = sc->name+CPE2_SYMBOL_SCOPE_SEP"reentry";
	sc->leadingSpace=currentScope->leadingSpace+tabLength;
	code->placeSymbol(SymbolType::CodeLocation,0,sc->reentrySymbol);
	code->placeSymbol(SymbolType::CodeLocation,0,sc->name);
	line conditionLine;
	conditionLine.lineNum = ParserState.Line.lineNum;
	conditionLine.file = ParserState.Line.file;
	variable* condition;
	ParserState.Token = ParserState.Line.nextToken();
	if(ParserState.Token.type == 30)
	{
		uint64_t l = 0;
		do {
			switch(ParserState.Token.type)
			{
				case(0):
					unexpectedBufferTermination("condition expression, logical line terminated",originCoreHere,source());
				case(30):
					l++;
					goto __default_wce;
				case(31):
					l--;
				default:
					__default_wce:;
					conditionLine.text += ParserState.Token.text+" ";
			}
			ParserState.Token = ParserState.Line.nextToken();
		} while(l > 0);
		conditionLine.text = conditionLine.text.substr(2,conditionLine.text.length()-4);
		token conditionToken = conditionLine.nextToken();
		condition = resolve(conditionToken);
		if(condition == nullptr)
			compilerBug("assertion failed: condition != nullptr");
	} 
	else 
		unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{30});
	if(currentArchitecture == Architecture::AMD64)
	{
		amd64::VariableStorage* cs = (amd64::VariableStorage*)condition->storage;
		if(cs->mode == amd64::StorageMode::DirectRegister)
		{
			code->push({
				amd64::prefix::REX(1,0,0,amd64::register_decode_rex(cs->reg)),
				amd64::opcode::cmp::rm16_32_64__imm8,
				amd64::modRM(0x7,amd64::AddressingMode::RegisterDirect,cs->reg),
				0
			});
			code->push({
				0x0F,
				amd64::opcode::secondary::jcc::rel16_32off(amd64::Condition::NotEqual),
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
						sc->name
					)
				);
			}
		}
		else
			compilerBug("unimplemented condition storage mode");
	}
	//prepare for body
	Entity::updateCurrentScope(sc);
	section* endcode = new section;
	endcode->push(currentScope->func->code);
	currentScope->func->code = new section;
	currentScope->extraCodeBlocks.push_back(currentScope->func->code);
	currentScope->extraCodeBlocks.push_back(endcode);
	code->placeSymbol(SymbolType::CodeLocation,0,currentScope->name);
}
