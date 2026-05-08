#pragma once
#include "math.hpp"
#include "miller_rabin.hpp"
#include "random.hpp"
#include "types.hpp"
#include <algorithm>
#include <cassert>
#include <concepts>
#include <numeric>
#include <utility>
#include <vector>

#ifndef __INT128
#error "pollard_rho requires 128-bit integer support"
#endif

namespace nnk {
template <std::unsigned_integral Int>
Int pollard_rho(Int n) {
  if (n == 1) return 1;
  const Int c = randint<Int>(1, n - 1);

  auto f = [&](Int x) {
    return (static_cast<u128>(x) * x % n + c) % n;
  };

  Int x, y = 2, z = 1, g = 1;
  const Int m = static_cast<Int>(1) << (floor_log2(n) / 5);
  for (Int r = 1; g == 1; r <<= 1) {
    x = y;
    for (Int i = 0; i < r; ++i) {
      y = f(y);
    }
    for (Int k = 0; k < r && g == 1; k += m) {
      z = y;
      Int prod = 1;
      for (Int i = 0; i < std::min<Int>(m, r - k); ++i) {
        y = f(y);
        Int diff = x > y ? x - y : y - x;
        prod = static_cast<u128>(prod) * diff % n;
      }
      g = std::gcd(prod, n);
    }
  }
  if (g == n) {
    do {
      z = f(z);
      Int diff = x > z ? x - z : z - x;
      g = std::gcd(diff, n);
    } while (g == 1);
  }
  return g;
}

template <std::unsigned_integral Int>
Int find_prime_factor(Int n) {
  assert(n > 1);
  if (miller_rabin(n)) return n;
  while (true) {
    const Int p = pollard_rho(n);
    if (miller_rabin(p)) return p;
    n = p;
  }
}

template <std::unsigned_integral Int>
auto factorization(Int n) {
  std::vector<std::pair<Int, int>> res;
  for (Int p = 2; p * p <= n && p < 100; ++p) {
    if (n % p) continue;
    int e = 0;
    do { n /= p, e++; } while (n % p == 0);
    res.emplace_back(p, e);
  }
  while (n > 1) {
    const Int p = find_prime_factor(n);
    int e = 0;
    do { n /= p, e++; } while (n % p == 0);
    res.emplace_back(p, e);
  }
  return res;
}
} // namespace nnk
