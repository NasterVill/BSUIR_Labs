#include "InvalidStackSizeException.h"

using namespace CustomThread;

InvalidStackSizeException::InvalidStackSizeException(const char* message) : exception(message)
{
}

InvalidStackSizeException::InvalidStackSizeException(std::string message) : exception(message.c_str())
{
}

InvalidStackSizeException::~InvalidStackSizeException()
{
}
