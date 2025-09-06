#pragma once
#include <mco/base_types.hpp>
#include <mco/io/stream.hpp>
#include <vector>

namespace mco {

	/// A memory stream.
	class MemoryStream : public WritableStream {
		// TODO: Support owned or leased memory.
		// In the case we are leasing memory, trying to write beyond
		// the end of the memory provided should always fail.
		std::vector<u8> buffer {};
		i64 seekPointer { 0 };

	   public:
		/// Default constructor.
		MemoryStream();

		/// Constructor for pre-allocating an inital size.
		explicit MemoryStream(u64 initialSize);

		/// Constructor. Writes data from a buffer into this stream.
		MemoryStream(const u8* sourceBuffer, usize sourceBufferSize);

		~MemoryStream();

		// Stream

		u64 read(void* buffer, u64 length) override;

		bool isRandomAccess() const noexcept override {
			return true;
		}

		i64 seek(i64 offset, Stream::Whence whence) override;
		i64 tell() override;

		u64 getSize() override;

		bool hasEnded() override;

		// WritableStream

		u64 write(const void* buffer, u64 length) override;

		// General methods

		/// Clear this MemoryStream.
		///
		/// The seek pointer will be set to 0,
		/// and the underlying memory that was backing the stream
		/// will be freed.
		void clear();
	};

} // namespace mco