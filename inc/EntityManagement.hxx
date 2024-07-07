#pragma once

#include <mangling.h>
#include <ABI.h>
#include <class_variable.h>

namespace Entity {
	enum class AttributeType {
		Invalid,
		Mangling,
		Local,
		Volatile,
		NoAlloc,
		Inline,
		Const,
		Constexpr,
		Extern,
		Noreturn,
		Typecast,
		ImplicitCast,
		ExplicitCast,
		Noop,
		Deprecated,
		ABI,
		Symbol,
		Primitive,
		Accessibility
	};
	struct PrimitiveAttributeData {
		primitiveOP Operation;
		bool InPlace;
		PrimitiveAttributeData(primitiveOP Operation,bool InPlace = false)
			:Operation(Operation),InPlace(InPlace){}
	};
	class Attribute {
	public:
		AttributeType Type;
		union {
			PrimitiveAttributeData Primitive;
			mangler* Mangler;
			ABI* Abi;
			std::string Symbol;
			vaccess Accessibility;
		};

		Attribute(AttributeType Type)
			:Type(Type){}

		Attribute(PrimitiveAttributeData data)
			:Primitive(data),Type(AttributeType::Primitive){}

		Attribute(mangler* Mangler)
			:Mangler(Mangler),Type(AttributeType::Mangling){}

		Attribute(ABI* Abi)
			:Abi(Abi),Type(AttributeType::ABI){}

		Attribute(std::string& Symbol)
			:Symbol(Symbol),Type(AttributeType::Symbol){}

		Attribute(vaccess Accessibility)
			:Accessibility(Accessibility),Type(AttributeType::Accessibility){}
	};

	bool close(scope* s);
}