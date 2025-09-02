#pragma once
#include <mco/io/stream.hpp>
#include <mco/pimple_container.hpp>

namespace mco {

	/// A random-access stream which repressents a OS file.
	struct FileStream : WritableStream {
		enum Mode {
			Read = (1 << 0),
			Write = (1 << 1),
			ReadWrite = Read | Write,
			Create = (1 << 2),
		};

		/// Opens a file stream.
		static FileStream open(const char* path, int mode = Mode::Read);

		FileStream(const FileStream&) = delete;
		FileStream(FileStream&& m);
		~FileStream();

		// Stream

		u64 read(void* buffer, u64 length) override;

		bool isRandomAccess() const noexcept override {
			return true;
		}

		i64 seek(i64 offset, Stream::Whence whence) override;
		i64 tell() override;
		void truncate(u64 size);

		/// Close this file, if it hasn't been closed
		/// or otherwise invalidated already
		void close();

		u64 getSize() override;

		//bool hasEnded() override;

		// WritableStream

		u64 write(const void* buffer, u64 length) override;

	   private:
		FileStream(); // Use the ::open() static method
		PimpleContainer<16> impl;
	};

} // namespace mco