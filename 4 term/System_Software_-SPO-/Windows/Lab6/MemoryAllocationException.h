#ifndef MEMORY_ALLOCATION_EXCEPTION_H
#define MEMORY_ALLOCATION_EXCEPTION_H

#include <stdexcept>
#include <string>
namespace memorymanager
{
	class MemoryAllocationException : public std::runtime_error
	{
	public:
		explicit MemoryAllocationException(const char* message);
		explicit MemoryAllocationException(std::string message);
		~MemoryAllocationException();
	};
}

#endif

