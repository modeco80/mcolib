//! Shared stream utilities and algorithms.
#pragma once
#include <mco/base_types.hpp>
#include <mco/io/stream.hpp>

namespace mco {
	/// Tees data from a read stream to another write stream.
	void teeStreams(Stream& source, WritableStream& dest, u64 teeLength);
} // namespace mco
