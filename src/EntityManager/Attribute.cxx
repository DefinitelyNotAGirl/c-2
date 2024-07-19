#include <EntityManagement.hxx>
#include <Parser.hxx>

void Entity::Attribute::init()
{
	this->Token = ParserState.Token;
}
