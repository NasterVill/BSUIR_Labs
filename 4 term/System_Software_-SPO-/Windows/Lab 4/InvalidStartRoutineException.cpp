#include "InvalidStartRoutineException.h"

using namespace CustomThread;

InvalidStartRoutineException::InvalidStartRoutineException(const char* message) : exception(message)
{
}

InvalidStartRoutineException::InvalidStartRoutineException(std::string message) : exception(message.c_str())
{
}

InvalidStartRoutineException::~InvalidStartRoutineException()
{
}