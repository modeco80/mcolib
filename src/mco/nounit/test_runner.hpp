#pragma once

namespace mco::nounit {
	class Test;
}

namespace mco::nounit::impl {
	/// Runs a single unit test.
	void runTest(Test* pTest);
} // namespace mco::nounit::impl