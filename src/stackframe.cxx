#include <stackframe.hxx>
#include <issues.hxx>
using namespace issues;

stackframe::stackframe()
{
}

stackframe::stackframe(uint64_t size)
	:size(size)
{
}

uint64_t stackframe::size()
{
	return this->size();
}

uint64_t stackframe::size(uint64_t bytes)
{
	this->size = bytes;
	if(this->workingOffset > this->size)
		compilerBug("stack overflow");
	return this->workingOffset;
}

uint64_t stackframe::push(uint64_t bytes)
{
	this->workingOffset += bytes;
	if((this->workingOffset+bytes) > (this->size))
		this->size = this->workingOffset+bytes;
}

uint64_t stackframe::pop(uint64_t bytes)
{
	if(this->workingOffset < bytes)
		compilerBug("stack underflow");
	this->workingOffset -= bytes;
}