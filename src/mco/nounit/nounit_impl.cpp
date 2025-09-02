#include <mco/nounit.hpp>

#include <cstdio>
#include <cstdlib>

namespace mco::nounit {
	TestState Test::getState() const {
		return mState;
	}
} // namespace nounit

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
				std::quick_exit(1000);
				break;
		}
	}

	void runTestImpl(Test* pTest) {
		if(!pTest)
			return;

		printf("======== nounit: executing test \"%s\"\n", pTest->description());
		try {
			pTest->execute();
		} catch(...) {
			///
		}

		// The test passed!
		if(pTest->getState() != TestState::Failed)
			pTest->mState = TestState::Passed;

		printf("======== nounit: test \"%s\" %s.\n", pTest->description(), testStateString(pTest->getState()));
	}

	void testFailureImpl(Test* pTest, const char* expression, const std::source_location& loc) {
		pTest->mState = TestState::Failed;
		printf("test failure\n");
		printf("reason: %s\n", expression);
		printf("source info: %s:%d\n", loc.file_name(), loc.line());
	}

	Test* testListHead { nullptr };

	void mainRegisterTest(Test* pTest) {
		if(pTest) {
			if(testListHead == nullptr) {
				testListHead = pTest;
				return;
			}

			// walk the list to the last empty entry
			Test* pIter = testListHead;
			while(pIter->mpNext != nullptr) {
				pIter = pIter->mpNext;
            }

			pIter->mpNext = pTest;
		}
	}

	int mainImpl() {
		int exitCode = 0;

		Test* pTest = testListHead;
		while(pTest != nullptr) {
			runTestImpl(pTest);
			// If any test fails the exit code is altered
			if(pTest->getState() == TestState::Failed)
				exitCode = 1;

			pTest = pTest->mpNext;
		}

		return exitCode;
	}

} // namespace nounit::impl