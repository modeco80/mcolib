//! A nano-sized unit testing library part of the mco base libraries.
//! Not intended to be particularly featureful or tasteful. Just work.
#pragma once

#include <format>
#include <source_location>
#include <string>

namespace mco::nounit {
	class Test;

	namespace impl {

		/// A class which holds both an unexpanded expression and the stringified
		/// result, ready for pretty-printer usage. Used by test internals, but needs
		/// to be public due to usage in nounit test assertion macros.
		class ExpandableExpression {
			const char* unexpanded;
			std::string expansion;

		   public:
			template <class T>
			explicit ExpandableExpression(const char* unexpanded, T& resultValue)
				: unexpanded(unexpanded) {
				this->expansion = std::format("{}", resultValue);
			}

			const char* getUnexpandedExpression() const {
				return unexpanded;
			}
			const std::string& getExpandedString() const {
				return expansion;
			}
		};

		std::string formatAssertNeqFailure(ExpandableExpression& resultExpression, ExpandableExpression& expectedExpression);
		std::string formatAssertEqFailure(ExpandableExpression& resultExpression, ExpandableExpression& expectedExpression);
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

/// Use inside of a test to assert that a expression should not throw.
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

/// Use inside of a test to assert an expression equals a result.
/// This is preferred to mcoNoUnitAssert(x == y) because this will expand the result of x and y
/// to be much more meaningful, which can help debugging why a unit test failed.
#define mcoNoUnitAssertEq(expr, result)                                                         \
	do {                                                                                        \
		auto x = expr;                                                                          \
		auto r = result;                                                                        \
		if(!(x == r)) {                                                                         \
			auto resExpr = ::mco::nounit::impl::ExpandableExpression(#expr, x);                 \
			auto expectedExpr = ::mco::nounit::impl::ExpandableExpression(#result, r);          \
			auto formatted = ::mco::nounit::impl::formatAssertEqFailure(resExpr, expectedExpr); \
			::mco::nounit::impl::testFailureImpl(this, formatted.c_str());                      \
		}                                                                                       \
	} while(0)

/// Use inside of a test to assert an expresssion does not equal a result.
#define mcoNoUnitAssertNeq(expr, result)                                                         \
	do {                                                                                         \
		auto x = expr;                                                                           \
		auto r = result;                                                                         \
		if(!(x != r)) {                                                                          \
			auto resExpr = ::mco::nounit::impl::ExpandableExpression(#expr, x);                  \
			auto expectedExpr = ::mco::nounit::impl::ExpandableExpression(#result, r);           \
			auto formatted = ::mco::nounit::impl::formatAssertNeqFailure(resExpr, expectedExpr); \
			::mco::nounit::impl::testFailureImpl(this, formatted.c_str());                       \
		}                                                                                        \
	} while(0)

/// Place this in your main test source file to generate a
/// main() which will run all defined tests.
#define mcoNoUnitMain()                         \
	int main(int argc, char** argv) {           \
		static_cast<void>(argc);                \
		static_cast<void>(argv);                \
		return ::mco::nounit::impl::mainImpl(); \
	}
