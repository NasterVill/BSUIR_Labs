#include "MemoryAllocationException.h"

using namespace memorymanager;

MemoryAllocationException::MemoryAllocationException(const char* message) : runtime_error(message)
{
}


MemoryAllocationException::MemoryAllocationException(std::string massage) : runtime_error(massage)
{
}

MemoryAllocationException::~MemoryAllocationException()
{
}

