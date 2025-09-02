#pragma once
#include <cstdint>

// Typedefs. I like them this way
using u8 = std::uint8_t;
using i8 = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
using usize = std::size_t;
using isize = std::ptrdiff_t;

/// A cutesy macro which makes static_assert struct size tests
/// a bit less annoying to write. The error message generated is
/// also a bit more informative.
#define mcoAssertSize(T, expected) \
    static_assert(sizeof(T) == expected, "Size of " #T " does not match expected: " #expected)
