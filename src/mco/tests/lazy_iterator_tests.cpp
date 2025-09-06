
#include <mco/nounit.hpp>
#include <mco/lazy_iterators.hpp>

mcoNoUnitDeclareTest(iotaTest, "basic iota() tests") {
    auto iter = mco::iota(1, 3);
    // test by manually iterating through the sequence that
    // iotaIterator works as expected
    mcoNoUnitAssertEq(iter.next().value(), 1);
    mcoNoUnitAssertEq(iter.next().value(), 2);
    mcoNoUnitAssertEq(iter.next().value(), 3);
    mcoNoUnitAssert(!iter.next().has_value());
}

mcoNoUnitDeclareTest(iotaIncrementTest, "iota() increment works") {
    auto iter = mco::iota(0, 16, 8);
    mcoNoUnitAssertEq(iter.next().value(), 0);
    mcoNoUnitAssertEq(iter.next().value(), 8);
    mcoNoUnitAssertEq(iter.next().value(), 16);
    mcoNoUnitAssert(!iter.next().has_value());
}

mcoNoUnitDeclareTest(mapTest, "test mapIterator()") {
    auto iotaIter = mco::iota(1, 16);
    auto mapIter = mco::map(iotaIter, [](auto item) {
        return (item % 8) == 0;
    });

    // test by manually iterating through the sequence that
    // mapIterator with our map function works as expected.
    mcoNoUnitAssertEq(mapIter.next().value(), 8);
    mcoNoUnitAssertEq(mapIter.next().value(), 16);
    mcoNoUnitAssert(!mapIter.next().has_value());
}

mcoNoUnitMain();