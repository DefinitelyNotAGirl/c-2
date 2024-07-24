#include <EntityManagement.hxx>
#include <output.hxx>
#include <Parser.hxx>

using namespace issues;

/**
 * @brief returns true if more bodies may need to be closed
 * 
 * @param ts 
 * @return true
 * @return false
 */
bool Entity::close(scope* ts)
{
	if (options::ddebug) std::cout << "body ended: " << ts->name << std::endl;
	for(Routine& r : ts->StartClosure)r.run();
	for(Routine& r : ts->Prologue)r.run();
	for(Routine& r : ts->BodyCode)r.run();
	for(Routine& r : ts->Epilogue)r.run();
	for(Routine& r : ts->Finalize)r.run();
	//for(Routine& r : ts->Destroy)r.run();
	//delete ts;
	return false;
}
