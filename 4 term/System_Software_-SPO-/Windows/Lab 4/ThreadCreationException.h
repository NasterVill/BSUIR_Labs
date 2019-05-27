#pragma once
#include <stdexcept>

namespace CustomThread
{
	class ThreadCreationException : public std::exception
	{
	public:
		explicit ThreadCreationException(std::string);
		explicit ThreadCreationException(const char*);

		~ThreadCreationException();
	};
};
