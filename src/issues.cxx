#define ISSUES_CXX
#include <issues.hxx>

namespace issues
{
	uint64_t ErrorCount = 0;

	void changeGroupAction(std::string name, action Action)
	{
		if(name == "absolute-memory")
		{
			absoluteMemoryStorage::Action = Action;
		}
		else if(name == "cpl")
		{
			insufficientPrivilegeLevel::Action = Action;
		}
		else if(name == "unimplemented")
		{
			unimplementedDebugInfo::Action = Action;
		}
	}
}
