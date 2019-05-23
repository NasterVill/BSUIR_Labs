#pragma once
#include <stdexcept>

namespace CustomThread
{
	class InvalidSectionException : public std::exception
	{
	public:
		explicit InvalidSectionException(std::string);
		explicit InvalidSectionException(const char*);

		~InvalidSectionException();
	};
};