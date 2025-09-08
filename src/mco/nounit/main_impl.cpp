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
			// If any test which is not allowed to fail fails, the exit code is altered.
			if(runTest(pTest) == 1)
				exitCode = 1;

			pTest = pTest->mpNext;
		}

		return exitCode;
	}

} // namespace mco::nounit::impl