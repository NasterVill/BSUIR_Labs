#ifndef ABSENT_MEMORY_MANAGER_EXCETION_H
#define ABSENT_MEMORY_MANAGER_EXCETION_H

#include <stdexcept>
#include <string>
namespace memorymanager
{
	class AbsentMemoryManagerException : std::logic_error
	{
	public:
		explicit AbsentMemoryManagerException(const char* message);
		explicit AbsentMemoryManagerException(std::string message);
		~AbsentMemoryManagerException();
	};
}

#endif