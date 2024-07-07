#include "parser.hxx"

void parse::Directive()
{
	if (t.text == "#EOL") {
		parse::EndLine();
	} else if (t.text == "#EOF") {
		parse::EndBlock();
	} else if (t.text == "#include") {
		parse::Directives::Include();
	} else if (t.text == "#pragma") {
		parse::Directives::Pragma();
	} else if (t.text == "#autodecl") {
		parse::Directives::Autodecl();
	}
}
