#include <stackframe.hxx>
#include <issues.hxx>
using namespace issues;

stackframe::stackframe()
{
}

stackframe::stackframe(uint64_t size)
	:m_size(size)
{
}

uint64_t stackframe::size()
{
	return this->m_size;
}

uint64_t stackframe::size(uint64_t bytes)
{
	this->m_size = bytes;
	if(this->m_workingOffset > this->m_size)
		compilerBug("stack overflow");
	return this->m_workingOffset;
}

uint64_t stackframe::push(uint64_t bytes)
{
	this->m_workingOffset += bytes;
	if((this->m_workingOffset+bytes) > (this->m_size))
		this->m_size = this->m_workingOffset+bytes;
	return this->m_workingOffset;
}

uint64_t stackframe::pop(uint64_t bytes)
{
	if(this->m_workingOffset < bytes)
		compilerBug("stack underflow");
	this->m_workingOffset -= bytes;
	return this->m_workingOffset;
}