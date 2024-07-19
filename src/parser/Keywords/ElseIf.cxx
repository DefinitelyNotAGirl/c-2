#include "../parser.hxx"

using namespace issues;

void parse::Keywords::ElseIf()
{
	if(options::ddebug)std::cout << "else if" << std::endl;
	scope* sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace+tabLength;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"elseif"+std::to_string(currentScope->elseIfCounter++);
	sc->parent = currentScope;
	sc->isIndentBased = true;
	sc->t = scopeType::CONDITIONAL_BLOCK;
	sc->func = new function;
	*(sc->func) = *(currentScope->func);
	//std::cout << "transfer " << currentScope->name << " -> " << sc->name << std::endl;
	sc->func->code = new section;
	sc->extraCodeBlocks.push_back(sc->func->code);
	sc->reentrySymbol = currentScope->name+CPE2_SYMBOL_SCOPE_SEP"conditional"+std::to_string(sc->parent->conditionalCounter)+CPE2_SYMBOL_SCOPE_SEP"reentry";
	//generate conditional jump code
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
					goto __default_eice;
				case(31):
					l--;
				default:
					__default_eice:;
					conditionLine.text += ParserState.Token.text+" ";
			}
			ParserState.Token = ParserState.Line.nextToken();
		} while(l > 0);
		conditionLine.text = conditionLine.text.substr(2,conditionLine.text.length()-4);
		std::cout << "condition: " << conditionLine.text << std::endl;
		token conditionToken = conditionLine.nextToken();
		condition = resolve(conditionToken);
		if(condition == nullptr)
			compilerBug("assertion failed: condition != nullptr");
		std::cout << "condition type: " << condition->dataType->name << std::endl;
	} 
	else 
		unexpectedTokenType("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),{30});
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
			compilerBug("unimplemented condition storage mode: "+std::to_string((uint64_t)cs->mode),originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),"");
	}
	//set return symbol
	Entity::updateCurrentScope(sc);
	code->placeSymbol(SymbolType::CodeLocation,0,sc->name);
}
