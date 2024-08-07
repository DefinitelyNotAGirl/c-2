#include "../parser.hxx"
using namespace issues;

void parse::Keywords::Return()
{
	if(options::ddebug)std::cout << "return" << std::endl;
	if(currentScope->t == scopeType::GLOBAL)
		invalidUseOfKeywordInScope("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),currentScope);
	ParserState.NextToken();
	variable* retVal = resolve(ParserState.Token);
	std::vector<variable*> args = {currentScope->func->returnValue,retVal};
	function* copyFunc = getFunction("operator=",args);
	call(copyFunc,args);
	if(currentArchitecture == Architecture::AMD64)
	{
		code->push({0xE9,0x00,0x00,0x00,0x00});
		code->Relocations.push_back(
			smu::RelocationEntry(
				code->size()-4,
				4,
				smu::RelocationType::Relative,currentScope->name+".epilogue"
			)
		);
	}
	else
		compilerBug("unimplemented: return, jump to epilogue");
}
