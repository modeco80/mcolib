#include <cstring>
#include <mco/io/memory_stream.hpp>

namespace mco {
	MemoryStream::MemoryStream() = default;

	MemoryStream::MemoryStream(u64 initialSize) {
		buffer.resize(initialSize);
	}

	MemoryStream::MemoryStream(const u8* sourceBuffer, usize sourceBufferSize) {
		buffer.resize(sourceBufferSize);
		memcpy(&buffer[0], sourceBuffer, sourceBufferSize);
	}

    MemoryStream::~MemoryStream() = default;

	u64 MemoryStream::read(void* buffer, u64 length) {
		// Well, you didn't ask me to read anything
		// so what makes you think I'm going to give you something.
		if(buffer == nullptr || length == 0)
			return 0;

		const auto remainder = this->buffer.size() - seekPointer;

		// We can't read any more data currently,
		// you're at the end of the stream.
		if(remainder == 0)
			return 0;

		if(length > remainder)
			length = remainder;

		// Copy data into the user's buffer and adjust the seek pointer.
		// The length is clamped beforehand, so this should be fine.
		memcpy(buffer, &this->buffer[seekPointer], length);
		seekPointer += length;

		return length;
	}

	u64 MemoryStream::write(const void* buffer, u64 length) {
		// I can't write nothing.
		if(buffer == nullptr || length == 0)
			return 0;

		if(const auto endExtent = seekPointer + length; endExtent > this->buffer.size()) {
			this->buffer.resize(endExtent);
		}

        memcpy(&this->buffer[seekPointer], buffer, length);
        seekPointer += length;
        return length;
	}

	i64 MemoryStream::seek(i64 offset, Stream::Whence whence) {
		i64 newPos = 0;

		switch(whence) {
			case Stream::Begin:
				newPos = offset;
				break;
			case Stream::Current:
				newPos = seekPointer + offset;
				break;
			case Stream::End:
				// end requires a negative offset
				if(offset > 0)
					return -1;
				newPos = buffer.size() + offset;
				break;
		}

		// Unlike file streams, seeking outside of the bounds
		// is *not* allowed. This is because that would be an
		// out of bounds memory access.
		if(newPos < 0 || newPos > static_cast<i64>(buffer.size()))
			return -1;
		seekPointer = newPos;
		return newPos;
	}

	i64 MemoryStream::tell() {
		return seekPointer;
	}

	u64 MemoryStream::getSize() {
		// The STL does a good enough job memoizing for us
		// so we don't need to do it here.
		return buffer.size();
	}

	bool MemoryStream::hasEnded() {
		return seekPointer == static_cast<i64>(buffer.size());
	}

    void MemoryStream::clear() {
        // Reset everything.
        buffer.clear();
        seekPointer = 0;
    }
} // namespace mco