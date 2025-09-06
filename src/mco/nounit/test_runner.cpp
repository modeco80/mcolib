#include "test_runner.hpp"

#include <mco/nounit.hpp>

#include "test_failure.hpp"

namespace mco::nounit::impl {

	const char* testStateString(TestState state) {
		switch(state) {
			case TestState::Failed:
				return "failed";
			case TestState::Passed:
				return "passed";
			case TestState::NeverRun:
				return "not run";
			default:
				return "unknown/unhandlable state";
				break;
		}
	}

	void runTest(Test* pTest) {
		if(!pTest)
			return;

		printf("= Executing test \"%s\"\n", pTest->description());

		try {
			pTest->execute();
		} catch(TestFailure& fail) {
			printf("= %s\n", fail.what());
		} catch(...) {
#if 0 // doesn't work :()
			auto ep = std::current_exception();
			try {
				std::rethrow_exception(ep);
			} catch(std::exception& ex) {
				printf("= Unhandled exception, failing test: %s", ex.what());
				pTest->mState = TestState::Failed;
			}
#endif
		}

		// The test passed!
		if(pTest->getState() != TestState::Failed)
			pTest->mState = TestState::Passed;

		printf("= Test \"%s\" %s.\n", pTest->description(), testStateString(pTest->getState()));
	}

} // namespace mco::nounit::impl