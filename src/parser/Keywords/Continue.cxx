/*

 */

#include "../parser.hxx"
#include <extint.hxx>
using namespace issues;

void parse::Keywords::Continue() {
	ParserState.NextToken();
	u64 BreakCount = 1;
	if(ParserState.Token.type != 41) {
		variable* count = resolve(ParserState.Token);
		if(count->storageArch != Architecture::storage_IntegerImmediate) {
			NonConstexprBreakCount("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		}
		BreakCount = (u64)count->storage;
	}
	scope* sc = currentScope;
	while(BreakCount > 0) {
		std::cout << "sc: " << sc->name << std::endl;
		std::cout << "BreakCount: " << BreakCount << std::endl;
		if(
			sc->t == scopeType::Loop
			|| sc->t == scopeType::Case
			|| sc->t == scopeType::If
			|| sc->t == scopeType::ElseIf
			|| sc->t == scopeType::Else
		){
			BreakCount--;
		}
		if(BreakCount > 0)
			sc = sc->parent;
	}
	//std::cout << "continue target: " << sc->reentrySymbol << std::endl;
	//std::cout << "continue target scope: " << sc->name << std::endl;
	runtime::RelativeControlTransfer(sc->reentrySymbol);
}
