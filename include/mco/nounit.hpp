//! A nano-sized unit testing library part of the mco base libraries.
//! Not intended to be particularly featureful. Maybe later it will gain
//! coroutines support. Hopefully not
#pragma once

#include <source_location>

namespace mco::nounit {
	class Test;

	namespace impl {
		void testFailureImpl(Test* pTest, const char* expression, const std::source_location& loc = std::source_location::current());
		void mainRegisterTest(Test* pTest);
		int mainImpl();
	} // namespace impl

	// public for exposition by impl only.
	enum class TestState { NeverRun,
						   Passed,
						   Failed };

	/// A test.
	class Test {
	   public:
		Test() {
			impl::mainRegisterTest(this);
		}

		TestState mState { TestState::NeverRun };
		Test* mpNext { nullptr };

		TestState getState() const;

		virtual ~Test() = default;
		virtual const char* description() const noexcept = 0;
		virtual void execute() = 0;
	};

} // namespace mco::nounit

/// This macro declares a test and its boilerplate for you.
/// After this macro you should implement the body of your test.
#define mcoNoUnitDeclareTest(name, desc)                    \
	class name : public ::mco::nounit::Test {               \
	   public:                                              \
		const char* description() const noexcept override { \
			return desc;                                    \
		}                                                   \
		void execute() override;                            \
	};                                                      \
	static name nounit__test__##name {};                    \
	void name::execute()

/// Use inside of a test to assert that a expression does not throw.
#define mcoNoUnitShouldNotThrow(expr)                                               \
	try {                                                                           \
		expr;                                                                       \
	} catch(...) {                                                                  \
		::mco::nounit::impl::testFailureImpl(this,                                  \
											 "the expression \"" #expr "\" threw"); \
	}

/// Use inside of a test to assert that an expression should throw.
#define mcoNoUnitShouldThrow(expr)                                               \
	do {                                                                         \
		bool threw = false;                                                      \
		try {                                                                    \
			expr;                                                                \
		} catch(...) {                                                           \
			threw = true;                                                        \
		}                                                                        \
                                                                                 \
		if(threw == false) {                                                     \
			::mco::nounit::impl::testFailureImpl(this, "the expression \"" #expr \
													   "\" did not throw");      \
		}                                                                        \
	} while(0)

/// Use inside of a test to assert something.
#define mcoNoUnitAssert(expr)                                                             \
	do {                                                                                  \
		if(!(expr)) {                                                                     \
			::mco::nounit::impl::testFailureImpl(this, "assertion \"" #expr "\" failed"); \
		}                                                                                 \
	} while(0)

/// Place this in your main test source file to generate a
/// capable main().
#define mcoNoUnitMain()                         \
	int main(int argc, char** argv) {           \
		static_cast<void>(argc);                \
		static_cast<void>(argv);                \
		return ::mco::nounit::impl::mainImpl(); \
	}
