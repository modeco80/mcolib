#pragma once

namespace mco::nounit {
	class Test;
}

namespace mco::nounit::impl {
	/// Runs a single unit test.
	/// Returns 1 if the test failed, 0 if it did not.
	int runTest(Test* pTest);
} // namespace mco::nounit::impl