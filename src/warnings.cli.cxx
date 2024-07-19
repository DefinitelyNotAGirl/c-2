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
			if(e.msg == "defaultUnsignedInt")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			if(e.msg == "defaultSignedInt")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			if(e.msg == "defaultChar")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			if(e.msg == "defaultWchar")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			if(e.msg == "defaultFloat")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			if(e.msg == "defaultBool")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			if(e.msg == "defaultPointer")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			if(e.msg == "stringifyable")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			if(e.msg == "iteratable")std::cout << " as of Sunday the 7th of July 2024, 11:33";
			std::cerr << "\n";
			e.src.print();
		}
	);

	unexpectedPrimitiveInPlace::warn.push(
		[](unexpectedPrimitiveInPlace e) {
			std::cerr << COLOR_WARN << "Warning" << COLOR_RESET << ": unexpected primitiveInPlace attribute";
			e.src.print();
			std::cerr << "\n\n\r";
		}
	);
}