#include <format>
#include <mco/nounit.hpp>
#include <string>

namespace mco::nounit::impl {

	std::string formatAssertNeqFailure(ExpandableExpression& resultExpression, ExpandableExpression& expectedExpression) {
		return std::format("{} (expansion: {}) equals {} (expansion: {})", resultExpression.getUnexpandedExpression(), resultExpression.getExpandedString(), expectedExpression.getUnexpandedExpression(), expectedExpression.getExpandedString());
	}

	std::string formatAssertEqFailure(ExpandableExpression& resultExpression, ExpandableExpression& expectedExpression) {
		return std::format("{} (expansion: {}) does not equal {} (expansion: {})", resultExpression.getUnexpandedExpression(), resultExpression.getExpandedString(), expectedExpression.getUnexpandedExpression(), expectedExpression.getExpandedString());
	}

} // namespace mco::nounit::impl