#pragma once
#include "modint.hpp"
#include <vector>

namespace nnk {
template <modint z>
struct factorial {
public:
  constexpr factorial() : m_data{1} {}
  constexpr z operator()(std::size_t p) {
    if (p >= m_data.size()) {
      extent(2 * p);
    }
    return m_data[p];
  }
  constexpr void reset() {
    m_data.clear();
    m_data.push_back(1);
  }

private:
  std::vector<z> m_data;

  constexpr void extent(std::size_t m) {
    std::size_t n = m_data.size();
    m_data.resize(m + 1);
    for (std::size_t i = n; i <= m; ++i) {
      m_data[i] = m_data[i - 1] * z{i};
    }
  }
};

template <modint z>
factorial<z> fact = factorial<z>{};

template <modint z>
struct inv_factorial {
public:
  constexpr inv_factorial() : m_data{1} {}
  constexpr z operator()(std::size_t p) {
    if (p >= m_data.size()) {
      extent(2 * p);
    }
    return m_data[p];
  }
  constexpr void reset() {
    m_data.clear();
    m_data.push_back(1);
  }

private:
  std::vector<z> m_data;

  constexpr void extent(std::size_t m) {
    std::size_t n = m_data.size();
    m_data.resize(m + 1);

    m_data[m] = fact<z>(m).inv();
    for (std::size_t i = m; i > n; --i) {
      m_data[i - 1] = m_data[i] * z{i};
    }
  }
};

template <modint z>
inv_factorial<z> ifact = inv_factorial<z>{};
} // namespace nnk
