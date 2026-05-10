#pragma once
#include "math.hpp"
#include <cassert>
#include <concepts>
#include <tuple>
#include <type_traits>
#include <utility>

namespace nnk {
template <std::integral T1, std::integral T2>
constexpr auto exgcd(T1 a, T2 b) noexcept {
  static_assert(!std::same_as<T1, bool> && !std::same_as<T2, bool>,
                "exgcd(a, b) does not support bool");

  using U = std::common_type_t<
      std::make_unsigned_t<T1>,
      std::make_unsigned_t<T2>,
      unsigned int
  >;
  using T = std::make_signed_t<U>;

  constexpr auto exgcd_core = [](U a, U b) -> std::tuple<T, T, U> {
    U x = 1, y = 0;
    U s = 0, t = 1;
    while (b) {
      U k = floor_div<U>(a, b);
      x -= k * s, y -= k * t, a -= k * b;
      std::swap(x, s), std::swap(y, t), std::swap(a, b);
    }
    return std::tuple<T, T, U>{
        static_cast<T>(x),
        static_cast<T>(y),
        a,
    };
  };

  if constexpr (std::is_signed_v<T1> && std::is_signed_v<T2>) {
    auto [x, y, d] = exgcd_core(
        a < 0 ? -static_cast<U>(a) : static_cast<U>(a),
        b < 0 ? -static_cast<U>(b) : static_cast<U>(b));
    if (a < 0) x = -x;
    if (b < 0) y = -y;
    return std::tuple<T, T, U>{x, y, d};
  } else if constexpr (std::is_unsigned_v<T1> && std::is_signed_v<T2>) {
    auto [x, y, d] = exgcd_core(
        static_cast<U>(a),
        b < 0 ? -static_cast<U>(b) : static_cast<U>(b));
    if (b < 0) y = -y;
    return std::tuple<T, T, U>{x, y, d};
  } else if constexpr (std::is_signed_v<T1> && std::is_unsigned_v<T2>) {
    auto [x, y, d] = exgcd_core(
        a < 0 ? -static_cast<U>(a) : static_cast<U>(a),
        static_cast<U>(b));
    if (a < 0) x = -x;
    return std::tuple<T, T, U>{x, y, d};
  } else {
    return exgcd_core(static_cast<U>(a), static_cast<U>(b));
  }
}
} // namespace nnk
