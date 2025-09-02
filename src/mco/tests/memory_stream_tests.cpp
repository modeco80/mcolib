#include <cstring>
#include <mco/io/memory_stream.hpp>
#include <mco/nounit.hpp>

mcoNoUnitDeclareTest(MemoryStreamConstructs, "mco::MemoryStream constructs and destructs in a sane manner") {
	mcoNoUnitShouldNotThrow(mco::MemoryStream stream);
}

mcoNoUnitDeclareTest(MemoryStreamBasicWrite, "mco::MemoryStream basic write test") {
	mco::MemoryStream stream;

    // test string
	constexpr const char str[] = "hello world";
	constexpr auto strLength = sizeof(str) - 1;

	// Write some data. This should expand the buffer to fit this data.
	auto n = stream.write(str, strLength);
	mcoNoUnitAssert(n == strLength);

	// Read it back and make sure it was not corrupted, and matches the input data.
	// This also tests that the logic for clamping the length works
	stream.seek(0, mco::Stream::Begin);
	char buf[128] {};
	n = stream.read(&buf[0], sizeof(buf));
	mcoNoUnitAssert(n == strLength);
	mcoNoUnitAssert(!memcmp(&buf[0], str, strLength));
}

mcoNoUnitMain()