#pragma once
#include "types.hpp"
#include <bit>
#include <concepts>
#include <initializer_list>
#include <limits>

#ifndef __INT128
#error "miller_rabin requires 128-bit integer support"
#endif

namespace nnk {
template <std::unsigned_integral Int>
bool miller_rabin(Int x) {
  if (x == 2 || x == 3 || x == 5 || x == 7) return true;
  if (x < 2 || x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0) return false;
  if (x < 121) return x > 1;
  const Int d = (x - 1) >> std::countr_zero<Int>(x - 1);

  auto check = [x, d](const Int& a) {
    Int y = 1, t = d;
    for (Int i = a, j = d; j; j >>= 1) {
      if (j & 1) y = static_cast<u128>(y) * i % x;
      i = static_cast<u128>(i) * i % x;
    }
    while (y != 1 && y != x - 1 && t != x - 1) {
      y = static_cast<u128>(y) * y % x;
      t <<= 1;
    }
    return y == x - 1 || t % 2 == 1;
  };

  if (x <= std::numeric_limits<u32>::max()) {
    for (const Int& a : {2, 7, 61}) {
      if (!check(a)) return false;
    }
  } else {
    for (const Int& a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
      if (!check(a)) return false;
    }
  }
  return true;
}
} // namespace nnk
