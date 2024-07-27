#include <EntityManagement.hxx>
#include <output.hxx>
#include <Parser.hxx>

using namespace issues;

/**
 * @brief returns true if a double close is required
 * 
 * @param ts 
 * @return true
 * @return false
 */
bool Entity::close(scope* ts)
{
	std::cout << "closing scope: " << ts->name << std::endl;
	for(Routine& r : ts->StartClosure)r.run();
	for(Routine& r : ts->Prologue)r.run();
	for(Routine& r : ts->BodyCode)r.run();
	for(Routine& r : ts->Epilogue)r.run();
	for(Routine& r : ts->Finalize)r.run();
	if(ts->parent->t == scopeType::TRY_CATCH)
	{
		line pslc = ParserState.Line;
		if(pslc.nextToken().text == "catch")
		{
			return false;
		}
		line psnlc = ParserState.Lines[ParserState.LineIterator+1];
		if(psnlc.nextToken().text == "catch")
		{
			return false;
		}
		std::cout << "detected end of try-catch clause, double closing scope" << std::endl;
		return true;
	}
	return false;
}
