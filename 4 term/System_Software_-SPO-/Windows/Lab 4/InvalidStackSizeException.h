#pragma once
#include <stdexcept>

namespace CustomThread
{
	class InvalidStackSizeException : public std::exception
	{
	public:
		explicit InvalidStackSizeException(std::string);
		explicit InvalidStackSizeException(const char*);

		~InvalidStackSizeException();
	};
};