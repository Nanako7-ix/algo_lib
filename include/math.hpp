#pragma once
#include "types.hpp"
#include <bit>
#include <cassert>
#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

namespace nnk {
template <std::integral Int>
constexpr Int floor_sqrt(Int n) {
  assert(n >= 0);
  if (n == 0) return 0;
  u64 t = std::sqrt(n);
  while (t > std::numeric_limits<u32>::max() || t * t > n) --t;
  while (t < std::numeric_limits<u32>::max() && (t + 1) * (t + 1) < n) ++t;
  return t;
}

template <std::integral Int>
constexpr Int ceil_sqrt(Int n) {
  assert(n >= 0);
  if (n == 0) return 0;
  u64 t = std::sqrt(n);
  while (t < std::numeric_limits<u32>::max() && t * t < n) ++t;
  while (t > std::numeric_limits<u32>::max() || (t - 1) * (t - 1) >= n) --t;
  return t;
}

template <std::integral Int>
constexpr int floor_log2(Int n) {
  assert(n >= 0);
  return std::bit_width<std::make_unsigned_t<Int>>(n) - 1;
}

template <std::integral Int>
constexpr int ceil_log2(Int n) {
  assert(n >= 0);
  if (n == 0) return 0;
  return std::bit_width<std::make_unsigned_t<Int>>(n - 1);
}

template <std::unsigned_integral Int>
constexpr Int floor_div(Int a, Int b) {
  assert(b != 0);
  return a / b;
}

template <std::unsigned_integral Int>
constexpr Int ceil_div(Int a, Int b) {
  assert(b != 0);
  if (a == 0) return 0;
  return (a - 1) / b + 1;
}

template <std::signed_integral Int>
constexpr Int floor_div(Int a, Int b) {
  assert(b != 0);
  if (b < 0) a = -a, b = -b;
  Int k = a / b;
  return k * b > a ? k - 1 : k;
}

template <std::signed_integral Int>
constexpr Int ceil_div(Int a, Int b) {
  assert(b != 0);
  if (b < 0) a = -a, b = -b;
  Int k = a / b;
  return k * b < a ? k + 1 : k;
}

template <typename T>
int sign(T x) {
  return x == 0 ? 0 : (x < 0 ? -1 : 1);
}

template <std::floating_point F>
auto solve_quadratic_equation(F a, F b, F c) -> std::optional<std::pair<F, F>> {
  assert(a != 0);
  F delta = b * b - 4 * a * c;
  if (delta == 0) {
    F x = -b / (2 * a);
    return {x, x};
  }
  if (delta < 0) return std::nullopt;
  F q = -(b + std::copysign<F>(std::sqrt(delta), b)) / 2;
  return {q / a, c / q};
}

template <std::integral T>
requires(!std::same_as<std::remove_cv_t<T>, bool>)
constexpr std::make_unsigned_t<T> safe_abs(T x) noexcept {
  using U = std::make_unsigned_t<T>;

  if constexpr (std::is_signed_v<T>) {
    U u = static_cast<U>(x);
    return x < 0 ? U{0} - u : u;
  } else {
    return x;
  }
}
} // namespace nnk
