#pragma once
#include "exgcd.hpp"
#include "modint.hpp"
#include "types.hpp"
#include <cassert>
#include <concepts>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace nnk {
template <std::unsigned_integral U, U Mod>
requires(std::numeric_limits<U>::digits <= 64)
class static_modint {
public:
  using value_type = std::common_type_t<U, unsigned int>;
  using signed_type = std::make_signed<value_type>;

  constexpr static value_type mod() noexcept {
    return Mod;
  }

  constexpr static_modint() : v(0) {}

  template <std::unsigned_integral T>
  constexpr static_modint(T x) noexcept {
    v = static_cast<value_type>(x % mod());
  }

  template <std::signed_integral T>
  constexpr static_modint(T x) noexcept {
    signed_type t = x % static_cast<signed_type>(mod());
    v = t < 0 ? t + mod() : t;
  }

  constexpr value_type val() const noexcept {
    return v;
  }

  constexpr static_modint operator-() const noexcept {
    return v == 0 ? 0 : mod() - v;
  }

  constexpr static_modint& operator+=(const static_modint& o) & noexcept {
    v += o.v;
    if (v >= mod()) v -= mod();
    return *this;
  }

  constexpr static_modint& operator-=(const static_modint& o) & noexcept {
    v -= o.v;
    if (v >= mod()) v += mod();
    return *this;
  }

  constexpr static_modint& operator*=(const static_modint& o) & noexcept {
    if constexpr (std::numeric_limits<value_type>::digits <= 32) {
      v = static_cast<u64>(v) * o.v % mod();
    } else {
#ifdef __INT128
      v = static_cast<u128>(v) * o.v % mod();
#else
      static_assert(std::numeric_limits<f128>::digits >= std::bit_width(Mod) + 2,
                    "Not Enough Precision");

      u64 k = std::round(static_cast<f128>(a) * b / Mod);
      u64 ret = a * b - k * Mod;
      if (ret >= Mod) ret += Mod;
      return ret;
#endif
    }
    return *this;
  }

  constexpr static_modint& operator/=(const static_modint& o) & {
    return *this *= o.inv();
  }

  friend constexpr static_modint operator+(static_modint lhs, const static_modint& rhs) noexcept {
    return lhs += rhs;
  }

  friend constexpr static_modint operator-(static_modint lhs, const static_modint& rhs) noexcept {
    return lhs -= rhs;
  }

  friend constexpr static_modint operator*(static_modint lhs, const static_modint& rhs) noexcept {
    return lhs *= rhs;
  }

  friend constexpr static_modint operator/(static_modint lhs, const static_modint& rhs) {
    return lhs /= rhs;
  }

  constexpr static_modint& operator++() & noexcept {
    *this += 1;
    return *this;
  }
  constexpr static_modint& operator--() & noexcept {
    *this -= 1;
    return *this;
  }
  constexpr static_modint operator++(int) noexcept {
    static_modint tmp = *this;
    *this += 1;
    return tmp;
  }
  constexpr static_modint operator--(int) noexcept {
    static_modint tmp = *this;
    *this -= 1;
    return tmp;
  }

  constexpr static_modint pow(u64 b) const noexcept {
    static_modint ret = 1, a = *this;
    for (; b; b >>= 1, a *= a) {
      if (b & 1) ret *= a;
    }
    return ret;
  }

  constexpr static_modint inv() const {
    auto [x, y, gcd] = exgcd(v, mod());
    if (gcd != 1) {
      throw std::invalid_argument("gcd(v, Mod) != 1, Inversion does not exist");
    }
    return static_modint{x};
  }

  friend constexpr auto operator<=>(const static_modint& lhs, const static_modint& rhs) noexcept = default;

  friend std::istream& operator>>(std::istream& is, static_modint& a) {
    i64 v;
    is >> v;
    a = static_modint(v);
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const static_modint& a) {
    os << a.v;
    return os;
  }

private:
  value_type v;
};

template <std::unsigned_integral U, U Mod>
requires(std::numeric_limits<U>::digits <= 64)
struct is_modint<static_modint<U, Mod>> : std::true_type {};
} // namespace nnk

template <std::unsigned_integral U, U Mod>
requires(std::numeric_limits<U>::digits <= 64)
class std::formatter<nnk::static_modint<U, Mod>> {
public:
  constexpr auto parse(format_parse_context& ctx) {
    return fmt.parse(ctx);
  }

  auto format(const nnk::static_modint<U, Mod>& x, format_context& ctx) const {
    return fmt.format(x.val(), ctx);
  }

private:
  std::formatter<typename nnk::static_modint<U, Mod>::value_type> fmt;
};
