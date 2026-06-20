#pragma once
#include "exgcd.hpp"
#include "math.hpp"
#include "modint.hpp"
#include "types.hpp"
#include <format>
#include <istream>
#include <stdexcept>

#if !defined(__INT128) && defined(_MSC_VER) && defined(_M_X64)
#include <intrin.h>
#endif

namespace nnk {
template <u32 id>
class dynamic_modint {
public:
  using value_type = u32;
  using signed_type = i32;

  constexpr static void set_mod(u32 m) {
    bt = barrett{m};
  }

  constexpr static value_type mod() noexcept {
    return bt.mod;
  }

  constexpr dynamic_modint() : v(0) {}

  template <std::unsigned_integral T>
  constexpr dynamic_modint(T x) noexcept {
    v = static_cast<value_type>(x % mod());
  }

  template <std::signed_integral T>
  constexpr dynamic_modint(T x) noexcept {
    auto r = safe_abs(x) % mod();
    v = x < 0 && r != 0 ? mod() - r : r;
  }

  constexpr value_type val() const noexcept {
    return v;
  }

  constexpr dynamic_modint operator-() const noexcept {
    return v == 0 ? 0 : mod() - v;
  }

  constexpr dynamic_modint& operator+=(const dynamic_modint& o) & noexcept {
    value_type r = mod() - o.v;
    if (v >= r) {
      v -= r;
    } else {
      v += o.v;
    }
    return *this;
  }

  constexpr dynamic_modint& operator-=(const dynamic_modint& o) & noexcept {
    if (v >= o.v) {
      v -= o.v;
    } else {
      v += mod() - o.v;
    }
    return *this;
  }

  constexpr dynamic_modint& operator*=(const dynamic_modint& o) & noexcept {
    v = bt.mul(v, o.v);
    return *this;
  }

  constexpr dynamic_modint& operator/=(const dynamic_modint& o) & {
    return *this *= o.inv();
  }

  friend constexpr dynamic_modint operator+(dynamic_modint lhs, const dynamic_modint& rhs) noexcept {
    return lhs += rhs;
  }

  friend constexpr dynamic_modint operator-(dynamic_modint lhs, const dynamic_modint& rhs) noexcept {
    return lhs -= rhs;
  }

  friend constexpr dynamic_modint operator*(dynamic_modint lhs, const dynamic_modint& rhs) noexcept {
    return lhs *= rhs;
  }

  friend constexpr dynamic_modint operator/(dynamic_modint lhs, const dynamic_modint& rhs) {
    return lhs /= rhs;
  }

  constexpr dynamic_modint& operator++() & noexcept {
    *this += 1;
    return *this;
  }
  constexpr dynamic_modint& operator--() & noexcept {
    *this -= 1;
    return *this;
  }
  constexpr dynamic_modint operator++(int) noexcept {
    dynamic_modint tmp = *this;
    *this += 1;
    return tmp;
  }
  constexpr dynamic_modint operator--(int) noexcept {
    dynamic_modint tmp = *this;
    *this -= 1;
    return tmp;
  }

  constexpr dynamic_modint pow(u64 b) const noexcept {
    dynamic_modint ret = 1, a = *this;
    for (; b; b >>= 1, a *= a) {
      if (b & 1) ret *= a;
    }
    return ret;
  }

  constexpr dynamic_modint inv() const {
    auto [x, y, gcd] = exgcd(v, mod());
    if (gcd != 1) {
      throw std::invalid_argument(
          std::format("gcd({}, {}) != 1, Inversion does not exist",
                      v, mod()));
    }
    return dynamic_modint{x};
  }

  friend constexpr auto operator<=>(const dynamic_modint& lhs, const dynamic_modint& rhs) noexcept = default;

  friend std::istream& operator>>(std::istream& is, dynamic_modint& a) {
    i64 v;
    is >> v;
    a = dynamic_modint(v);
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const dynamic_modint& a) {
    os << a.v;
    return os;
  }

private:
  struct barrett {
    u32 mod;
    u64 inv_mod;

    constexpr barrett(u32 m = 998244353) {
      if (m == 0) {
        throw std::invalid_argument("Mod Can Not Euals To 0");
      }
      mod = m;
      inv_mod = static_cast<u64>(-1) / mod + 1;
    }

    constexpr u32 mul(u32 a, u32 b) const noexcept {
      u64 x = static_cast<u64>(a) * b;
      u64 k = 0;

#ifdef __INT128
      k = static_cast<u64>((static_cast<u128>(x) * inv_mod) >> 64);
#elif defined(_MSC_VER) && defined(_M_X64)
      if (std::is_constant_evaluated()) {
        return x % mod;
      }
      _umul128(x, inv_mod, &k);
#else
      return x % mod;
#endif

      u64 y = k * mod;
      return static_cast<u32>(x - y + (x < y ? mod : 0));
    }
  };
  static barrett bt;

  value_type v;
};

template <u32 id>
dynamic_modint<id>::barrett dynamic_modint<id>::bt = {};

template <u32 id>
struct is_modint<dynamic_modint<id>> : std::true_type {};
} // namespace nnk

template <nnk::u32 id>
class std::formatter<nnk::dynamic_modint<id>> {
public:
  constexpr auto parse(format_parse_context& ctx) {
    return fmt.parse(ctx);
  }

  auto format(const nnk::dynamic_modint<id>& x, format_context& ctx) const {
    return fmt.format(x.val(), ctx);
  }

private:
  std::formatter<typename nnk::dynamic_modint<id>::value_type> fmt;
};
