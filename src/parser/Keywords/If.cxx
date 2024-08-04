#include "../parser.hxx"
#include <conditions.hxx>

using smu::InstructionComponent;
using smu::InstructionComponentType;
using namespace issues;


void parse::Keywords::If() {
	if(currentScope->t == scopeType::GLOBAL)
		invalidUseOfKeywordInScope("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),currentScope);
	if(options::ddebug)std::cout << "if" << std::endl;
	scope* sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace+tabLength;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"if"+std::to_string(currentScope->ifCounter++);
	sc->parent = currentScope;
	sc->isIndentBased = ParserState.Line.text.back() == ':';
	sc->t = scopeType::If;
	sc->func = new function;
	sc->parent->conditionalCounter++;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	sc->reentrySymbol = currentScope->name+CPE2_SYMBOL_SCOPE_SEP"conditional"+std::to_string(sc->parent->conditionalCounter)+CPE2_SYMBOL_SCOPE_SEP"reentry";
	sc->extraCodeBlocks.push_back(sc->func->code);
	//generate conditional jump code
	line cl = ParserState.Line;
	cl.text = "";
	cl.tpos = 0;
	token cond;
	ParserState.NextToken();
	if(ParserState.Token.type == 30) {
		cond = ParserState.Token;
		for(char c : ParserState.Line.restText())
		{
			switch(c)
			{
				case(')'):
					goto endCLine0;
				default:
					cl.text.push_back(c);
			}
		}
		endCLine0:;
	} else {
		unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{30});
	}
	//std::cout << "condition: " << cl.text << std::endl;
	cond = cl.nextToken();
	//std::cout << "if condition check" << std::endl;
	variable* condition = resolve(cond);
	if(ConditionCode.top() == Condition::True) {
		//std::cout << "if(true)" << std::endl;
		runtime::RelativeControlTransfer(ImmediateValue(sc->name+CPE2_SYMBOL_SCOPE_SEP+"body"));
	} else if(ConditionCode.top() == Condition::False) {
	} else if(currentArchitecture == Architecture::AMD64) {
		code->push(std::vector<InstructionComponent>({
			0x0F,
			::amd64::opcode::secondary::jcc::rel16_32off((amd64::Condition)ConditionCode.top()),
			smu::RelocationEntry(0,4,smu::RelocationType::Relative,sc->name+CPE2_SYMBOL_SCOPE_SEP+"body")
		}));
	}
	code->placeSymbol(SymbolType::CodeLocation,0,sc->reentrySymbol);
	struct RoutineData_T {
		scope* sc;
	};
	RoutineData_T* RoutineData = new RoutineData_T; {
		RoutineData->sc = sc;
	}
	Entity::updateCurrentScope(sc);
	currentScope->BranchCode.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code->placeSymbol(SymbolType::CodeLocation,0,data->sc->name+CPE2_SYMBOL_SCOPE_SEP+"body");
			code->push(data->sc->func->code);
			runtime::RelativeControlTransfer(ImmediateValue(data->sc->reentrySymbol));
		}
	));
	currentScope->Finalize.push_back(
		Routine(
			RoutineData,
			[](void* __data){
				RoutineData_T* data = (RoutineData_T*)__data;
				for(Routine& r : data->sc->BranchCode) {
					data->sc->parent->BranchCode.push_back(r);
				}
			}
		)
	);
}
