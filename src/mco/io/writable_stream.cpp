#include <mco/io/stream.hpp>

namespace mco {

	bool WritableStream::isWritable() const noexcept {
		return true;
	}

	/// Write some data to this stream.
	/// Returns the amount of bytes actually placed.
	u64 WritableStream::write(const void* buffer, u64 length) {
		static_cast<void>(buffer);
		static_cast<void>(length);
		return -1;
	}

	void WritableStream::put(u8 c) {
		write(&c, sizeof(c));
	}
} // namespace mco