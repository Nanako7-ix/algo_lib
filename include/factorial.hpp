#pragma once
#include "modint.hpp"
#include "types.hpp"
#include <stdexcept>
#include <vector>

namespace nnk {
template <modint z>
struct factorial {
public:
  constexpr factorial() : n(0), f{1} {}
  constexpr z operator()(std::size_t p) {
    if (p > n) extent(2 * p);
    return f[p];
  }
  constexpr void clear() {
    f.clear();
  }

private:
  std::size_t n;
  std::vector<z> f;

  constexpr void extent(std::size_t m) {
    f.resize(m + 1);
    for (std::size_t i = n; i < m; ++i) {
      f[i + 1] = f[i] * z{i + 1};
    }
    n = m;
  }
};

template <modint z>
factorial<z> fact = factorial<z>{};

template <modint z>
struct inv_factorial {
public:
  constexpr inv_factorial() : n(0), g{1} {}
  constexpr z operator()(std::size_t p) {
    if (p > n) extent(2 * p);
    return g[p];
  }
  constexpr void clear() {
    g.clear();
  }

private:
  std::size_t n;
  std::vector<z> g;

  constexpr void extent(std::size_t m) {
    g.resize(m + 1);
    g[m] = fact<z>(m).inv();

    for (std::size_t i = m; i > n; --i) {
      g[i - 1] = g[i] * z{i};
    }
    n = m;
  }
};

template <modint z>
inv_factorial<z> ifact = inv_factorial<z>{};
} // namespace nnk
