#include <mco/io/file_stream.hpp>

// Win32 implementation
#ifdef _WIN32
	#error Currently no win32 version implemented sorry
#endif

// POSIX implementation
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__)) || (defined(__APPLE__) && defined(__MACH__))
	#include "file_stream_impl.posix.cpp"
#endif

namespace mco {

	/*static */ FileStream FileStream::open(const char* path, int mode) {
		FileStream stream;
		stream.impl.getType<impl::FileStreamImpl>().open(path, mode);
		return stream;
	}

	FileStream::FileStream() {
		impl.constructType<impl::FileStreamImpl>();
	}

	FileStream::FileStream(FileStream&& other) {
		impl.constructType<impl::FileStreamImpl>(std::move(other.impl.getType<impl::FileStreamImpl>()));
	}

	FileStream::~FileStream() {
		impl.destructType<impl::FileStreamImpl>();
	}

	u64 FileStream::read(void* buffer, u64 length) {
		if(buffer == nullptr || length == 0)
			return 0;
		return impl.getType<impl::FileStreamImpl>().read(buffer, length);
	}

	u64 FileStream::write(const void* buffer, u64 length) {
		if(buffer == nullptr || length == 0)
			return 0;
		return impl.getType<impl::FileStreamImpl>().write(buffer, length);
	}

	i64 FileStream::seek(i64 offset, Stream::Whence whence) {
		return impl.getType<impl::FileStreamImpl>().seek(offset, whence);
	}

	i64 FileStream::tell() {
		return impl.getType<impl::FileStreamImpl>().tell();
	}

	void FileStream::truncate(u64 size) {
		return impl.getType<impl::FileStreamImpl>().truncate(size);
	}

	void FileStream::close() {
		impl.getType<impl::FileStreamImpl>().close();
	}

	u64 FileStream::getSize() {
		return impl.getType<impl::FileStreamImpl>().getSize();
	}
} // namespace mco