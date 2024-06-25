#include <issues.hxx>
#include <colors.h>
using namespace issues;

void warn_init()
{
	unimplementedDebugInfo::warn.push(
		[](unimplementedDebugInfo e) {
        	std::cerr << COLOR_WARN << "Warning" << COLOR_RESET << ": " << e.msg << " not implemented\n";
			e.src.print();
		}
	);

	insufficientPrivilegeLevel::warn.push(
		[](insufficientPrivilegeLevel e) {
			std::cerr << COLOR_WARN << "Warning" << COLOR_RESET << ": insufficient cpu privilege to " << e.msg << "\n";
			e.src.print();
		}
	);
}