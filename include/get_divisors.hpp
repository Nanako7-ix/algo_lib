#pragma once
#include <concepts>
#include <vector>

namespace nnk {
template <std::unsigned_integral Int>
auto get_divisors(const std::vector<std::pair<Int, int>>& pf) -> std::vector<Int> {
  std::vector<Int> res = {1};
  for (const auto& [p, e] : pf) {
    std::size_t siz = res.size();
    for (std::size_t i = 0; i < siz; ++i) {
      Int x = 1;
      for (int j = 0; j < e; ++j) {
        x *= p;
        res.push_back(res[i] * x);
      }
    }
  }
  return res;
}
} // namespace nnk
