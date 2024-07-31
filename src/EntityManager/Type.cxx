#include <EntityManagement.hxx>
#include <class_scope.h>
#include <Parser.hxx>
#include <output.hxx>

using Entity::AttributeType;
using Entity::Attribute;

using namespace issues;


static type* constructType(std::vector<Attribute> attributes,std::string& name, std::vector<token>& inherit)
{
	type* Type = new type;
	Type->name = name;
	Type->size = 0;
	//+
	//+ check attributes
	//+
	{
		for(Attribute& attr : attributes)
		{
			switch(attr.Type)
			{
				default:
					invalidAttribute(
						"",originCoreHere,source(ParserState.File,*attr.Token.Line,attr.Token),
						"class",attr.Token.text,
						{
						}
					);
			}
		}
	}
	for(token& ti : inherit)
	{
		std::string& i = ti.text;
		//+
		//+ check for primitive types
		//+
		{
			uint64_t psize = uint64_t_max;
			if(i == "primitive0")
				psize = 0;
			else if(i == "primitive8")
				psize = 1;
			else if(i == "primitive16")
				psize = 2;
			else if(i == "primitive32")
				psize = 4;
			else if(i == "primitive64")
				psize = 8;
			else if(i == "primitiveFloat32")
				psize = 4;
			else if(i == "primitiveFloat64")
				psize = 8;
			else
				goto nspt;
			Type->size += psize;
			continue;
		}
		nspt:;
		//+
		//+ check for type
		//+
		{
			type* T = getType(ti.text);
			if(T != nullptr)
			{
				Type->supertypes.push_back(T);
				for(variable m : T->members)
				{
					m.storage = (void*)Type->size;
					Type->members.push_back(m);
					Type->size += m.dataType->size;
				}
				continue;
			}
		}
		issues::noSuchType("",originCoreHere,source(ParserState.File,*ti.Line,ti),ti.text);
	}
	Type->incomplete = true;
	getMangler("c+=2")->mangle(Type);

	types.push_back(Type);
	importExternalValue(Type->getRuntimeTypeID());
	resourceCode.push_back("c2resource typeid "+Type->mangledName+";");
	return Type;
}

type* Entity::startTypeDefinition(std::vector<Attribute>& attributes, std::string& name, std::vector<token>& inherit, bool isIndentBased)
{
	type* Type = constructType(attributes,name,inherit);
	//+
	//+ construct new scope
	//+
	{
		scope* sc		  = new scope;
		sc->parent		  = currentScope;
		sc->name = currentScope->name + CPE2_SYMBOL_SCOPE_SEP +Type->mangledName;
		sc->leadingSpace = ParserState.Line.leadingSpaces + tabLength;
		sc->isIndentBased = true;
		sc->func = nullptr;
		sc->t			  = scopeType::CLASS;
		sc->cl			  = Type;
		updateCurrentScope(sc);	
		struct RoutineData_T {
			type* Type;
		};
		RoutineData_T* RoutineData = new RoutineData_T;{
			RoutineData->Type = Type;
		}
		currentScope->Finalize.push_back(Routine(RoutineData,
			[](void* __data){
				RoutineData_T* data = (RoutineData_T*)__data;
				data->Type->incomplete = false;
			}
		));
	}
	return Type;
}

type* Entity::declareType(std::vector<Attribute>& attributes,std::string name, std::vector<token> inherit)
{
	return constructType(attributes,name,inherit);
}
