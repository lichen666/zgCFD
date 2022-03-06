#pragma once
#include <exception>
#include <string>

namespace zgCFD {
	class matrix_csr_exception : public std::exception
	{
		std::string message;

	public:

		explicit matrix_csr_exception(const std::string& message) :
			exception(),
			message(message)
		{}

		std::string getMessage(void) const
		{
			return message;
		}
	};
}

