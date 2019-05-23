#include "InvalidSectionException.h"

using namespace CustomThread;

InvalidSectionException::InvalidSectionException(const char* message) : exception(message)
{
}

InvalidSectionException::InvalidSectionException(std::string message) : exception(message.c_str())
{
}

InvalidSectionException::~InvalidSectionException()
{
}
