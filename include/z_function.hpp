#pragma once
#include <iterator>
#include <vector>

namespace nnk {
template <std::random_access_iterator Iter>
auto z_function(Iter first, Iter last) -> std::vector<int> {
  std::size_t n = std::distance(first, last);
  if (n == 0) return {};

  std::vector<int> z(n);
  z[0] = n;
  for (std::size_t i = 1, j = 0; i < n; ++i) {
    z[i] = std::max<int>(0, std::min<int>(j + z[j] - i, z[i - j]));
    while (i + z[i] < n && first[i + z[i]] == first[z[i]]) ++z[i];
    if (i + z[i] > j + z[j]) j = i;
  }
  return z;
}
} // namespace nnk
