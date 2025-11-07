#include <cstring>
#include <mco/io/gzip_in_stream.hpp>

namespace mco {
    
	GzipInStream::GzipInStream(Stream& sourceStream)
		: wrappedStream(sourceStream) {
		// Allocate the temporary read buffer.
		pInflateReadBuffer = new u8[kInflateReadBufferSize];

		// Initalize the stream data.
		memset(&zStream, 0, sizeof(zStream));
		zStream.zalloc = Z_NULL;
		zStream.zfree = Z_NULL;
		zStream.opaque = Z_NULL;

		// 47 window bits enable auto-detection of either Zlib or Gzip
		// format compressed data.
		zlibError = inflateInit2(&zStream, 47);
	}

	GzipInStream::~GzipInStream() {
		inflateEnd(&zStream);
		delete[] pInflateReadBuffer;
	}

	u64 GzipInStream::read(void* buffer, u64 length) {
		if(eof)
			return 0;

		// Tell the stream about the new output buffer.
		zStream.avail_out = static_cast<uInt>(length);
		zStream.next_out = static_cast<Bytef*>(buffer);

		while(zStream.avail_out > 0 && !eof) {
			// If the inflate has processed a full buffer, then read a new chunk of data
			// from the underlying stream and tell zlib about it.
			if(zStream.avail_in == 0) {
				auto bytesRead = wrappedStream.read(pInflateReadBuffer, kInflateReadBufferSize);
				if(bytesRead == 0) {
					eof = true;
					break;
				}
				zStream.avail_in = static_cast<uInt>(bytesRead);
				zStream.next_in = static_cast<Bytef*>(pInflateReadBuffer);
			}

			// Inflate some data.
			zlibError = inflate(&zStream, Z_NO_FLUSH);
			if(zlibError == Z_STREAM_END) {
				eof = true;
				break;
			} else if(zlibError != Z_OK) {
				// Zlib error. Indicate as such
				return -1;
			}
		}

		return static_cast<u64>(length - zStream.avail_out);
	}

	int GzipInStream::getZError() const {
		return zlibError;
	}

	const char* GzipInStream::getZErrorMsg() const {
		return zStream.msg;
	}

} // namespace mco
