#pragma once
#include <mco/base_types.hpp>
#include <string>

namespace mco {
    /// Formats a size in bytes to a more reasonable human-readable
    /// repressentation (also converts units when appropiate).
    std::string makeHumanReadableByteSize(u64 bytes);
}