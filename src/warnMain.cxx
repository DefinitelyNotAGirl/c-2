#include <issues.hxx>
#include <colors.h>
using namespace issues;

void warn_init()
{
	unimplementedDebugInfo::handle.push(
		[](unimplementedDebugInfo e) {
        	std::cerr << COLOR_WARN << "Warning" << COLOR_RESET << ": " << e.msg << " not implemented\n";
			e.src.print();
		}
	);
}