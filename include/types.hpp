#pragma once
#include <cstdint>

namespace nnk {
using i32  = std::int32_t;
using i64  = std::int64_t;
using u32  = std::uint32_t;
using u64  = std::uint64_t;
using f32  = float;
using f64  = double;
using f128 = long double;

#ifdef __SIZEOF_INT128__
#define __INT128
using i128 = __int128;
using u128 = unsigned __int128;
#endif
} // namespace nnk
