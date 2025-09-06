#include <mco/nounit.hpp>
#include "test_runner.hpp"

namespace mco::nounit::impl {

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
			runTest(pTest);
			// If any test fails the exit code is altered
			if(pTest->getState() == TestState::Failed)
				exitCode = 1;

			pTest = pTest->mpNext;
		}

		return exitCode;
	}

} // namespace mco::nounit::impl