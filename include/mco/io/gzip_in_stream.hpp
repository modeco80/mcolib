#pragma once

#include <zlib.h>
#include <mco/io/stream.hpp>

namespace mco {
    
    /// An adapter stream which reads compressed Gzip or Zlib data from a input stream,
    /// and produces uncompressed data when read from.
	class GzipInStream : public Stream {
		/// The wrapped stream.
		Stream& wrappedStream;

		// Zlib state.
		z_stream zStream;
		int zlibError = 0;
		bool eof = false;

		// Temporary inflate buffer.
		u8* pInflateReadBuffer;
		constexpr static auto kInflateReadBufferSize = 4096;

	   public:
		GzipInStream(Stream& sourceStream);

		// Disallow copy and movement. This makes the implementation a bit simpler,
		// and heavily decreases the risk of misuse (mco streams are meant to be passed
		// by-reference anyways).
		GzipInStream(const GzipInStream&) = delete;
		GzipInStream(GzipInStream&&) = delete;

		~GzipInStream();

		u64 read(void* buffer, u64 length) override;

		int getZError() const;

		const char* getZErrorMsg() const;
	};

} // namespace mco