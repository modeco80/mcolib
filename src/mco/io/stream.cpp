#include <mco/io/stream.hpp>

namespace mco {
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

	void WritableStream::put(u8 c) {
		write(&c, sizeof(c));
	}
} // namespace mco