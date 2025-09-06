#pragma once
#include <exception>
#include <source_location>
#include <string>
#include <format>

namespace mco::nounit::impl {

	class TestFailure : public std::exception {
		std::string message;

	   public:
		TestFailure(const char* expression, const std::source_location& loc) {
			message = std::format("Test FAILURE @ {}:{}: {}", loc.file_name(), loc.line(), expression);
		}

		const char* what() const noexcept override {
			return message.c_str();
		}
	};

} // namespace mco::nounit::impl