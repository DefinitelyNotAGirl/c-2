#include "../parser.hxx"
#include <conditions.hxx>

using smu::InstructionComponent;
using smu::InstructionComponentType;
using namespace issues;

void parse::Keywords::Else()
{
	ParserState.NextToken();
	if(ParserState.Token.text == "if")
		return parse::Keywords::ElseIf();
	if(options::ddebug)std::cout << "else" << std::endl;
	scope* sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace+tabLength;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"else"+std::to_string(currentScope->elseIfCounter++);
	sc->parent = currentScope;
	sc->isIndentBased = ParserState.Line.text.back() == ':';
	sc->t = scopeType::CONDITIONAL_BLOCK;
	sc->func = new function;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	sc->reentrySymbol = currentScope->name+CPE2_SYMBOL_SCOPE_SEP"conditional"+std::to_string(sc->parent->conditionalCounter)+CPE2_SYMBOL_SCOPE_SEP"reentry";
	sc->extraCodeBlocks.push_back(sc->func->code);
	u64 SymbolIterator = 0;
	for(cgu::Symbol& s : code->symbols) {
		if(s.name == sc->reentrySymbol) {
			code->symbols.erase(code->symbols.begin() + SymbolIterator);
		}
		SymbolIterator++;
	}
	struct RoutineData_T {
		scope* sc;
	};
	RoutineData_T* RoutineData = new RoutineData_T; {
		RoutineData->sc = sc;
	}
	Entity::updateCurrentScope(sc);
	currentScope->StartClosure.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code = new section;
		}
	));
	currentScope->BodyCode.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code->placeSymbol(SymbolType::CodeLocation,0,data->sc->name+CPE2_SYMBOL_SCOPE_SEP+"body");
			//code->push(amd64::opcode::nop);
			code->push(data->sc->func->code);
			code->placeSymbol(SymbolType::CodeLocation,0,data->sc->reentrySymbol);
		}
	));
	currentScope->Finalize.push_back(
		Routine(
			RoutineData,
			[](void* __data){
				RoutineData_T* data = (RoutineData_T*)__data;
				data->sc->parent->func->code->push(code);
				for(Routine& r : data->sc->BranchCode) {
					data->sc->parent->BranchCode.push_back(r);
				}
			}
		)
	);
}
