#include "../parser.hxx"
#include <SMU.h>
#include <conditions.hxx>

using namespace issues;
using smu::InstructionComponent;
using smu::InstructionComponentType;

void parse::Keywords::For()
{
	//std::cout << "keyword: for" << std::endl;
	ParserState.Token = ParserState.Line.nextToken();
	//construct new scope
	scope* sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace+tabLength;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"forloop"+std::to_string(currentScope->forCounter++);
	sc->parent = currentScope;
	sc->isIndentBased = true;
	sc->t = scopeType::Loop;
	sc->func = new function;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	line beginLine = ParserState.Line;
	beginLine.text = ParserState.Line.restText();
	beginLine.tpos = 0;
	beginLine.leadingSpaces+=tabLength;
	std::vector<line> beginLines = {beginLine};
	line conditionLine = ParserState.Lines[++ParserState.LineIterator];
	token cond = conditionLine.nextToken();
	line endLine = ParserState.Lines[++ParserState.LineIterator];
	{
		uint64_t bi = endLine.text.size()-1;
		while(endLine.text[bi] != ':' && endLine.text[bi] != '{')
			bi--;
		sc->isIndentBased = (endLine.text[bi] == ':');
		while(endLine.text[bi] != ')')
			bi--;
		endLine.text = endLine.text.substr(0,bi);
	}
	endLine.leadingSpaces = currentScope->leadingSpace;
	std::vector<line> endLines = {endLine};
	Entity::updateCurrentScope(sc);
	//+
	//+	parse begin line
	//+
	{
		code->placeSymbol(SymbolType::CodeLocation,0,sc->name+CPE2_SYMBOL_SCOPE_SEP+"prologue");
		parse::Lines(beginLines);
	}
	//+
	//+	parse condition line
	//+
	{
		code->placeSymbol(SymbolType::CodeLocation,0,sc->name+CPE2_SYMBOL_SCOPE_SEP+"continue");
		variable* condition = resolve(cond);
		if((u16)ConditionCode.top() == 0xF001) {
			runtime::RelativeControlTransfer(ImmediateValue(sc->name+CPE2_SYMBOL_SCOPE_SEP+"body"));
		} else if((u16)ConditionCode.top() == 0xF000) {
		} else if(currentArchitecture == Architecture::AMD64) {
			code->push(std::vector<InstructionComponent>({
				0x0F,
				::amd64::opcode::secondary::jcc::rel16_32off((amd64::Condition)ConditionCode.top()),
				smu::RelocationEntry(0,4,smu::RelocationType::Relative,sc->name+CPE2_SYMBOL_SCOPE_SEP+"body")
			}));
		}
		code->placeSymbol(SymbolType::CodeLocation,0,scope::join({sc->name,"break"}));
	}
	//+
	//+	parse end line
	//+
	section* endcode = new section;
	{
		section* rc = code;
		code = endcode;
		code->placeSymbol(SymbolType::CodeLocation,0,sc->name+CPE2_SYMBOL_SCOPE_SEP+"epilogue");
		parse::Lines(endLines);
		runtime::RelativeControlTransfer(ImmediateValue(sc->name+CPE2_SYMBOL_SCOPE_SEP+"continue"));
		code = rc;
	}
	struct RoutineData_T {
		section* precode;
		section* body;
		section* endcode;
		scope* sc;
	};
	RoutineData_T* RoutineData = new RoutineData_T; {
		RoutineData->sc = sc;
		RoutineData->endcode = endcode;
		RoutineData->precode = code;
		currentScope->func->code = new section;
		RoutineData->body = currentScope->func->code;
	}
	code = currentScope->func->code;
	currentScope->StartClosure.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code = new section;
		}
	));
	currentScope->BodyCode.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code->push(data->precode);
		}
	));
	currentScope->BranchCode.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code->placeSymbol(SymbolType::CodeLocation,0,data->sc->name+CPE2_SYMBOL_SCOPE_SEP+"body");
			code->push(data->body);
			code->push(data->endcode);
		}
	));
	currentScope->Finalize.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			data->sc->parent->func->code->push(code);
			for(Routine& r : data->sc->BranchCode)
				data->sc->parent->BranchCode.push_back(r);
		}
	));
	currentScope->Destroy.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
		}
	));
}
