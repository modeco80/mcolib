#pragma once
#include <exception>
#include <source_location>
#include <string>

namespace mco::nounit::impl {

	class TestFailure : public std::exception {
		std::string message;

	   public:
		TestFailure(const char* expression, const std::source_location& loc);

		const char* what() const noexcept override {
			return message.c_str();
		}
	};

} // namespace mco::nounit::impl