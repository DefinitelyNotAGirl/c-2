#include "parser.hxx"

using Entity::Attribute;
using Entity::AttributeType;
using Entity::PrimitiveAttributeData;

void parse::AttributeList()
{
	while (ParserState.Token.type >= 20 && ParserState.Token.type < 31) {
		if(ParserState.Token.type == 30)
		{
			ParserState.NextToken();
			std::string text = "("+ParserState.Token.text;
			recheckLspecAttribEnd:;
			ParserState.NextToken();
			if(ParserState.Token.type == 31)
				text+=")";
			else if(ParserState.Token.type == 1)
			{
				text+=ParserState.Token.text;
				goto recheckLspecAttribEnd;
			}
			if(ParserState.Token.type != 31)
				unexpectedTokenType("",originCoreHere,source(currentFile,ParserState.Token,ParserState.Token),{31});
			ParserState.Token.text = text;
		}
			 if(ParserState.Token.text == "public"      )ParserState.Attributes.push_back(Attribute(vaccess      ::_public     ));
		else if(ParserState.Token.text == "protected"   )ParserState.Attributes.push_back(Attribute(vaccess      ::_protected  ));
		else if(ParserState.Token.text == "private"     )ParserState.Attributes.push_back(Attribute(vaccess      ::_private    ));
		else if(ParserState.Token.text == "local"       )ParserState.Attributes.push_back(Attribute(AttributeType::Local       ));
		else if(ParserState.Token.text == "volatile"    )ParserState.Attributes.push_back(Attribute(AttributeType::Volatile    ));
		else if(ParserState.Token.text == "noalloc"     )ParserState.Attributes.push_back(Attribute(AttributeType::NoAlloc     ));
		else if(ParserState.Token.text == "inline"      )ParserState.Attributes.push_back(Attribute(AttributeType::Inline      ));
		else if(ParserState.Token.text == "const"       )ParserState.Attributes.push_back(Attribute(AttributeType::Const       ));
		else if(ParserState.Token.text == "constexpr"   )ParserState.Attributes.push_back(Attribute(AttributeType::Constexpr   ));
		else if(ParserState.Token.text == "extern"      )ParserState.Attributes.push_back(Attribute(AttributeType::Extern      ));
		else if(ParserState.Token.text == "noreturn"    )ParserState.Attributes.push_back(Attribute(AttributeType::Noreturn    ));
		else if(ParserState.Token.text == "typecast"    )ParserState.Attributes.push_back(Attribute(AttributeType::Typecast    ));
		else if(ParserState.Token.text == "implicitcast")ParserState.Attributes.push_back(Attribute(AttributeType::ImplicitCast));
		else if(ParserState.Token.text == "explicitcast")ParserState.Attributes.push_back(Attribute(AttributeType::ExplicitCast));
		else if(ParserState.Token.text == "noop"        )ParserState.Attributes.push_back(Attribute(AttributeType::Noop        ));
		else if(ParserState.Token.text == "deprecated"  )ParserState.Attributes.push_back(Attribute(AttributeType::Deprecated  ));
		else if(ParserState.Token.text.substr(0,strlen("SYMBOL-")) == "SYMBOL-")ParserState.Attributes.push_back(Attribute(ParserState.Token.text.substr(strlen("SYMBOL-")),AttributeType::Symbol));
		else if(ParserState.Token.text.substr(0,strlen("ABI-")) == "ABI-")ParserState.Attributes.push_back(Attribute(getABI(ParserState.Token.text.substr(strlen("ABI-")))));
		else if(ParserState.Token.text.substr(0,strlen("mangling-")) == "mangling-")ParserState.Attributes.push_back(Attribute(getMangler(ParserState.Token.text.substr(strlen("mangling-")))));
		else if(ParserState.Token.text == "defaultUnsignedInt")deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "defaultSignedInt"  )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "defaultChar"       )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "defaultWchar"      )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "defaultFloat"      )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "defaultBool"       )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "defaultPointer"    )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "nodoc"             )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "export"            )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "iteratable"        )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "stringifyable"     )deprecatedAttribute(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		else if(ParserState.Token.text == "primitiveSYSCALL"      )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::SYSCALL     )));
		else if(ParserState.Token.text == "primitivePRINTCHAR"    )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::PRINTCHAR   )));
		else if(ParserState.Token.text == "primitivePRINTSTR"     )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::PRINTSTR    )));
		else if(ParserState.Token.text == "primitiveInterrupt"    )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::Interrupt   )));
		else if(ParserState.Token.text == "primitiveArrayIndex"   )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::Index       )));
		else if(ParserState.Token.text == "primitiveAdd"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::add         )));
		else if(ParserState.Token.text == "primitiveSub"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::sub         )));
		else if(ParserState.Token.text == "primitiveMul"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::mul         )));
		else if(ParserState.Token.text == "primitiveDiv"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::div         )));
		else if(ParserState.Token.text == "primitiveMod"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::mod         )));
		else if(ParserState.Token.text == "primitiveEqual"        )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::equal       )));
		else if(ParserState.Token.text == "primitiveNotEqual"     )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::NotEqual    )));
		else if(ParserState.Token.text == "primitiveGreater"      )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::Greater     )));
		else if(ParserState.Token.text == "primitiveGreaterEqual" )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::GreaterEqual)));
		else if(ParserState.Token.text == "primitiveLess"         )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::Less        )));
		else if(ParserState.Token.text == "primitiveLessEqual"    )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::LessEqual   )));
		else if(ParserState.Token.text == "primitiveAnd"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::_and        )));
		else if(ParserState.Token.text == "primitiveXor"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::_xor        )));
		else if(ParserState.Token.text == "primitiveOr"           )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::_or         )));
		else if(ParserState.Token.text == "primitiveNot"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::Not         )));
		else if(ParserState.Token.text == "primitiveInc"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::Inc         )));
		else if(ParserState.Token.text == "primitiveDec"          )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::Dec         )));
		else if(ParserState.Token.text == "primitiveAssign"       )ParserState.Attributes.push_back(Attribute(PrimitiveAttributeData(primitiveOP::assign      )));
		else if(ParserState.Token.text == "primitiveInPlace") {
			if(ParserState.Attributes.back().Type == AttributeType::Primitive)
				ParserState.Attributes.back().Primitive.InPlace = true;
			else
				unexpectedPrimitiveInPlace("",originCoreHere,source());
		} else if(ParserState.Token.text.front() == '(' && ParserState.Token.text.back() == ')') {
			if(ParserState.Token.text[1] == '+' || ParserState.Token.text[1] == '-') {
				line L = ParserState.Line;
				L.ccol = 0;
				L.tpos = 0;
				if(ParserState.Token.text[1] == '+')
					L.text = ParserState.Token.text.substr(2,ParserState.Token.text.length()-3);
				else
					L.text = ParserState.Token.text.substr(1,ParserState.Token.text.length()-2);
				token t = L.nextToken();
				variable* offs = resolve(t);
				if(offs->storageArch != Architecture::storage_IntegerImmediate)
					compilerBug("unimplemented attribute: "+ParserState.Token.text);
				ParserState.Attributes.push_back(Attribute(int64_t(offs->storage)));
			} else if(isdigit(ParserState.Token.text[1])) {
				line L = ParserState.Line;
				L.ccol = 0;
				L.tpos = 0;
				L.text = ParserState.Token.text.substr(1,ParserState.Token.text.length()-2);
				token t = L.nextToken();
				variable* offs = resolve(t);
				if(offs->storageArch != Architecture::storage_IntegerImmediate)
					compilerBug("unimplemented attribute: "+ParserState.Token.text);
				ParserState.Attributes.push_back(Attribute(uint64_t(offs->storage)));
			} else {
				ParserState.Attributes.push_back(Attribute(ParserState.Token.text.substr(1,ParserState.Token.text.length()-2),AttributeType::RegisterStorage));
			}
		}
		else
			compilerBug("unimplemented attribute: "+ParserState.Token.text);
		if(ParserState.Attributes.size() > 0)
			ParserState.Attributes.back().Token = ParserState.Token;
		ParserState.NextToken();
	}
}
