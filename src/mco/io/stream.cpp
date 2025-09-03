#include <mco/io/stream.hpp>

namespace mco {

	u64 Stream::read(void* buffer, u64 length) {
		static_cast<void>(buffer);
		static_cast<void>(length);
		return -1;
	}

	/// Returns true if this stream is random access
	/// (i.e: it can be rewound repeatedly).
	/// If false, seek/tell() will always return -1.
	bool Stream::isRandomAccess() const noexcept {
		return false;
	}

	bool Stream::isWritable() const noexcept {
		return false;
	}

	i64 Stream::tell() {
		return -1;
	}

	i64 Stream::seek(i64 offset, Whence whence) {
		static_cast<void>(offset);
		static_cast<void>(whence);
		return -1;
	}

	u64 Stream::getSize() {
		return -1;
	}

	bool Stream::hasEnded() {
		return false;
	}

	/// Like [seek()], but returns a scope object that will
	/// rewind the stream to the same place it was before this
	/// seek was performed. Handy and makes certain seek patterns
	/// shorter and harder to introduce bugs in.
	Stream::Rewinder Stream::seekTemporary(i64 offset, Whence whence) {
		Rewinder rewinder(*this);
		seek(offset, whence);
		return rewinder;
	}

	u8 Stream::get() {
		u8 c;
		read(&c, sizeof(c));
		return c;
	}

	std::string Stream::readString() {
		std::string str;
		while(true) {
			auto c = static_cast<char>(get());
			if(c == '\0')
				break;
			str += c;
		}
		return str;
	}

	
} // namespace mco