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

	deprecatedAttribute::warn.push(
		[](deprecatedAttribute e) {
			std::cerr << COLOR_WARN << "Warning" << COLOR_RESET << ": attribute " << e.msg << " has been deprecated";
			if(e.msg == "nodoc")std::cout << " as of Thursday the 4th of July 2024, 22:54";
			if(e.msg == "export")std::cout << " as of Thursday the 4th of July 2024, 22:54";
			std::cout << "\n";
			e.src.print();
		}
	);
}