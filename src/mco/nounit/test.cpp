#include <mco/nounit.hpp>
#include "test_failure.hpp"

namespace mco::nounit {
	TestState Test::getState() const {
		return mState;
	}

	namespace impl {
		void testFailureImpl(Test* pTest, const char* expression, const std::source_location& loc) {
			pTest->mState = TestState::Failed;
			throw TestFailure(expression, loc);
		}
	} // namespace impl
} // namespace mco::nounit
