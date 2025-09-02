#include <format>
#include <mco/io/file_stream.hpp>

#include <mco/nounit.hpp>
#include <cstring>

// Just here to make accessing them less awful and clearer
struct testGlobals {
	/// The test directory.
	std::string testDirectory;

	std::string makeTestFilename(const std::string& filename) {
		return std::format("{}/{}", testDirectory, filename);
	}

	testGlobals() {
		// TODO: Ideally this globals instance should manage
		// a temporary directory that on destruction always gets destroyed.
		// Currently nounit will squash exceptions
		testDirectory = "/tmp";
	}
};

testGlobals globals;

constexpr char writeMessage[] = "Write test\n";
constexpr auto writeMessageLength = sizeof(writeMessage) - 1;

mcoNoUnitDeclareTest(FileStreamBasicWrite, "FileStream handles writes gracefully") {
	auto filename = globals.makeTestFilename("write_test.txt");
	auto stream = mco::FileStream::open(filename.c_str(), mco::FileStream::ReadWrite | mco::FileStream::Create);
	stream.truncate(0);

	// Excercise the stream
	auto nWritten = stream.write(reinterpret_cast<const std::uint8_t*>(&writeMessage[0]), writeMessageLength);
	mcoNoUnitAssert(nWritten == writeMessageLength);
}

mcoNoUnitDeclareTest(FileStreamBasicRead, "FileStream handles reads gracefully") {
	// Open the file written to in the past test, except in read-only mode.
	auto filename = globals.makeTestFilename("write_test.txt");
	auto stream = mco::FileStream::open(filename.c_str(), mco::FileStream::Read);

	// Try to read the data. 
	// This should not completely fill the buffer, but instead match the length 
	// of what we initially wrote. Additionally, it should actually match.
	char buf[128] {};
	auto nWritten = stream.read(&buf[0], sizeof(buf));
	mcoNoUnitAssert(nWritten == writeMessageLength);
	mcoNoUnitAssert(!memcmp(&buf[0], &writeMessage[0], writeMessageLength));
}

mcoNoUnitDeclareTest(FileStreamMoveTest, "FileStream handles C++ movement correctly") {
	auto movementReciever = [this](mco::FileStream&& s) {
		auto movedStream = std::move(s);
		constexpr char msg[] = "Moving a stream works without closing the file abruptly\n";
		constexpr auto msgLen = sizeof(msg) - 1;
		auto nWritten = movedStream.write(reinterpret_cast<const std::uint8_t*>(&msg[0]), msgLen);
		mcoNoUnitAssert(nWritten == msgLen);
	};

	auto filename = globals.makeTestFilename("move_test.txt");
	auto stream = mco::FileStream::open(filename.c_str(), mco::FileStream::ReadWrite | mco::FileStream::Create);
	stream.truncate(0);

	// Excercise the stream
	constexpr char msg[] = "test\n";
	constexpr auto msgLen = sizeof(msg) - 1;
	auto nWritten = stream.write(reinterpret_cast<const std::uint8_t*>(&msg[0]), msgLen);
	mcoNoUnitAssert(nWritten == msgLen);

	// Now move the stream. The moved function should correctly 
	// own the stream and thus should close the file when it returns.
	movementReciever(std::move(stream));
}

mcoNoUnitMain()