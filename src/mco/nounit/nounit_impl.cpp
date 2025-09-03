#include <cstdio>
#include <cstdlib>
#include <exception>
#include <format>
#include <mco/nounit.hpp>
#include <string>

namespace mco::nounit {
	TestState Test::getState() const {
		return mState;
	}
} // namespace mco::nounit

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

	std::string formatAssertNeqFailure(ExpandableExpression& resultExpression, ExpandableExpression& expectedExpression) {
		return std::format("{} (expansion: {}) equals {} (expansion: {})", resultExpression.getUnexpandedExpression(), resultExpression.getExpandedString(), expectedExpression.getUnexpandedExpression(), expectedExpression.getExpandedString());
	}

	std::string formatAssertEqFailure(ExpandableExpression& resultExpression, ExpandableExpression& expectedExpression) {
		return std::format("{} (expansion: {}) does not equal {} (expansion: {})", resultExpression.getUnexpandedExpression(), resultExpression.getExpandedString(), expectedExpression.getUnexpandedExpression(), expectedExpression.getExpandedString());
	}

	void runTestImpl(Test* pTest) {
		if(!pTest)
			return;

		printf("= Executing test \"%s\"\n", pTest->description());
		try {
			pTest->execute();
		} catch(TestFailure& fail) {
			printf("= %s\n", fail.what());
		} catch(...) {
			///
		}

		// The test passed!
		if(pTest->getState() != TestState::Failed)
			pTest->mState = TestState::Passed;

		printf("= Test \"%s\" %s.\n", pTest->description(), testStateString(pTest->getState()));
	}

	void testFailureImpl(Test* pTest, const char* expression, const std::source_location& loc) {
		pTest->mState = TestState::Failed;
		throw TestFailure(expression, loc);
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

} // namespace mco::nounit::impl