//! A hilariously simplistic stream abstraction library thing.
//! Meant to basically be an anti-iostreams, because
//! iostreams IMO kind of isn't the best.
#pragma once

#include <mco/base_types.hpp>
#include <string>

namespace mco {

	struct Stream {
		enum Whence {
			Begin,
			Current,
			End
		};

		virtual ~Stream() = default;

		/// Read some data from the stream
		/// Returns the amount of bytes taken from the stream.
		virtual u64 read(void* buffer, u64 length);

		/// Read a single byte.
		u8 get();

		/// Reads a null-terminated string from the stream.
		std::string readString();

		/// Returns true if this stream is random access
		/// (i.e: it can be rewound repeatedly).
		/// If false, seek/tell() will always return -1.
		virtual bool isRandomAccess() const noexcept;

		/// Returns true if the stream is writable.
		/// If true, this stream can be upcasted to [WritableStream]
		/// and written to. If false, it can not.
		virtual bool isWritable() const noexcept;

		/// Obtain the internal seek pointer.
		/// If [isRandomAccess()] returns false, this function will always return -1.
		virtual i64 tell();

		/// Advance the stream in a random-access fashion.
		/// If [isRandomAccess()] returns false, this function will always return -1.
		virtual i64 seek(i64 offset, Whence whence);

		/// If this stream is random-access, obtains the size of it.
		virtual u64 getSize();

		/// If this stream is random-access, returns if the stream has ended.
		virtual bool hasEnded();

		struct Rewinder {
			constexpr Rewinder(Stream& stream) : stream(stream), lastOffset(stream.tell()) {
			}

			~Rewinder() {
				stream.seek(lastOffset, Stream::Begin);
			}

		   private:
			Stream& stream;
			i64 lastOffset;
		};

		/// Like [seek()], but returns a scope object that will
		/// rewind the stream to the same place it was before this
		/// seek was performed. Handy and makes certain seek patterns
		/// shorter and harder to introduce bugs in.
		Rewinder seekTemporary(i64 offset, Whence whence);
	};

	/// A stream which supports writing.
	struct WritableStream : Stream {
		bool isWritable() const noexcept override;
		
		/// Write some data to this stream.
		/// Returns the amount of bytes actually placed.
		virtual u64 write(const void* buffer, u64 length);

		/// Put a single byte.
		void put(u8 c);
	};

} // namespace mco