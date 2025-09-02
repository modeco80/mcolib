#include <mco/io/stream_utils.hpp>

namespace mco {
	/// The chunk size that teeStreams should use. Here because
	/// it's an implementation detail and not something that should be exposed.
	constexpr static auto kTeeChunkSize = 2048;

	void teeStreams(Stream& source, WritableStream& dest, u64 nToTee) {
		u8 buffer[kTeeChunkSize] {};

		// Calculate how many chunks it will take to tee the amount of bytes
		// we have been requested to.
		//
		// Since it may not be evenly divisible into chunks,
		// we also need to calculate the remainder
		u64 nChunks = nToTee / kTeeChunkSize;
		u64 nRemainder = nToTee % kTeeChunkSize;

		// Do as much as we can in chunks of [kTeeChunkSize] bytes.
		for(u64 i = 0; i < nChunks; ++i) {
			auto n = source.read(&buffer[0], kTeeChunkSize);
			dest.write(&buffer[0], n);
		}

		// Once we've written all we can as chunks, we need to
		// write the final remainder, if there is any.
		if(nRemainder) {
			auto n = source.read(&buffer[0], nRemainder);
			dest.write(&buffer[0], n);
		}
	}

} // namespace mco