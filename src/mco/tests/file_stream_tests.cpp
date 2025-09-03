#include <cstring>
#include <format>
#include <mco/io/file_stream.hpp>
#include <mco/nounit.hpp>
#include <vector>

#ifdef __unix__
	#include <unistd.h>
#endif

// Just here to make accessing them less awful and clearer
class FileStreamTestGlobals {
	/// The test directory.
	std::string testDirectory;

	/// Paths to cleanup at the end of the test.
	std::vector<std::string> cleanupPaths;

   public:
	std::string makeTestFilename(const std::string& filename) {
		auto path = std::format("{}/{}", testDirectory, filename);
		cleanupPaths.push_back(path);
		return path;
	}

	FileStreamTestGlobals() {
#ifdef __unix__
		testDirectory = "/tmp";
#endif
	}

	~FileStreamTestGlobals() {
		// Remove files that the test created.
		for(auto& path : cleanupPaths) {
#ifdef __unix__
			unlink(path.c_str());
#endif
		}
	}
};

FileStreamTestGlobals testGlobals;

constexpr char writeMessage[] = "Write test\n";
constexpr auto writeMessageLength = sizeof(writeMessage) - 1;

mcoNoUnitDeclareTest(FileStreamBasicWrite, "FileStream handles writes gracefully") {
	auto filename = testGlobals.makeTestFilename("write_test.txt");
	auto stream = mco::FileStream::open(filename.c_str(), mco::FileStream::ReadWrite | mco::FileStream::Create);
	stream.truncate(0);

	// Excercise the stream
	auto nWritten = stream.write(reinterpret_cast<const std::uint8_t*>(&writeMessage[0]), writeMessageLength);
	mcoNoUnitAssertEq(nWritten, writeMessageLength);
}

mcoNoUnitDeclareTest(FileStreamBasicRead, "FileStream handles reads gracefully") {
	// Open the file written to in the past test, except in read-only mode.
	auto filename = testGlobals.makeTestFilename("write_test.txt");
	auto stream = mco::FileStream::open(filename.c_str(), mco::FileStream::Read);

	// Try to read the data.
	// This should not completely fill the buffer, but instead match the length
	// of what we initially wrote. Additionally, it should actually match.
	char buf[128] {};
	auto nWritten = stream.read(&buf[0], sizeof(buf));
	mcoNoUnitAssertEq(nWritten, writeMessageLength);
	mcoNoUnitAssert(!memcmp(&buf[0], &writeMessage[0], writeMessageLength));
}

mcoNoUnitDeclareTest(FileStreamMoveTest, "FileStream handles C++ movement correctly") {
	auto movementReciever = [this](mco::FileStream&& s) {
		auto movedStream = std::move(s);
		constexpr char msg[] = "Moving a stream works without closing the file abruptly\n";
		constexpr auto msgLen = sizeof(msg) - 1;
		auto nWritten = movedStream.write(reinterpret_cast<const std::uint8_t*>(&msg[0]), msgLen);
		mcoNoUnitAssertEq(nWritten, msgLen);
	};

	auto filename = testGlobals.makeTestFilename("move_test.txt");
	auto stream = mco::FileStream::open(filename.c_str(), mco::FileStream::ReadWrite | mco::FileStream::Create);
	stream.truncate(0);

	// Excercise the stream
	constexpr char msg[] = "test\n";
	constexpr auto msgLen = sizeof(msg) - 1;
	auto nWritten = stream.write(reinterpret_cast<const std::uint8_t*>(&msg[0]), msgLen);
	mcoNoUnitAssertEq(nWritten, msgLen);

	// Now move the stream. The moved function should correctly
	// own the stream and thus should close the file when it returns.
	movementReciever(std::move(stream));
}

mcoNoUnitMain()