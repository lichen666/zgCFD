#pragma once
#include <exception>
#include <string>

namespace zgCFD {
	class base_field_exception : public std::exception
	{
		std::string message;

	public:

		explicit base_field_exception(const std::string& message) :
			exception(),
			message(message)
		{}

		std::string getMessage(void) const
		{
			return message;
		}
	};
}