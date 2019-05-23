#include "ThreadCreationException.h"

using namespace CustomThread;

ThreadCreationException::ThreadCreationException(const char* message) : exception(message)
{
}

ThreadCreationException::ThreadCreationException(std::string message) : exception(message.c_str())
{
}

ThreadCreationException::~ThreadCreationException()
{
}