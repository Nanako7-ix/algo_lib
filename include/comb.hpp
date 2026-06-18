#pragma once
#include "factorial.hpp"
#include "modint.hpp"

namespace nnk {
template <modint z>
struct mod_comb {
  constexpr static z A(int n, int m) {
    if (n < 0 || m < 0 || n < m) {
      return z(0);
    }
    return fact<z>(n) * ifact<z>(n - m);
  }
  constexpr static z C(int n, int m) {
    if (n < 0 || m < 0 || n < m) {
      return z(0);
    }
    return fact<z>(n) * ifact<z>(n - m) * ifact<z>(m);
  }
  constexpr static z inv(u32 v) {
    if (v == 0) {
      throw std::invalid_argument("Inverse Dose Not Exist.");
    }
    return fact<z>(v - 1) * ifact<z>(v);
  }
};
} // namespace nnk
