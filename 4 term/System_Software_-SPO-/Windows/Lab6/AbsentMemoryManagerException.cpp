#include "AbsentMemoryManagerException.h"

using namespace memorymanager;

AbsentMemoryManagerException::AbsentMemoryManagerException(const char* message) : logic_error(message)
{
}

AbsentMemoryManagerException::AbsentMemoryManagerException(std::string massage) : logic_error(massage)
{
}

AbsentMemoryManagerException::~AbsentMemoryManagerException()
{
}
