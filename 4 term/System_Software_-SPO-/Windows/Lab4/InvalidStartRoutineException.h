#pragma once
#include <stdexcept>

namespace CustomThread
{
	class InvalidStartRoutineException : public std::exception
	{
	public:
		explicit InvalidStartRoutineException(std::string);
		explicit InvalidStartRoutineException(const char*);

		~InvalidStartRoutineException();
	};
};

