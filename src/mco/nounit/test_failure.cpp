#include "test_failure.hpp"
#include <format>

namespace mco::nounit::impl {
	TestFailure::TestFailure(const char* expression, const std::source_location& loc) {
		message = std::format("Test FAILURE @ {}:{}: {}", loc.file_name(), loc.line(), expression);
	}
} // namespace mco::nounit::impl