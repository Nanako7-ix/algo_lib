#pragma once
#include <algorithm>
#include <array>
#include <concepts>
#include <iterator>
#include <random>

namespace nnk {
static std::mt19937 rng(std::random_device{}());
static std::mt19937_64 rng_64(std::random_device{}());

template <std::integral Int>
Int randint(Int l, Int r) {
  std::uniform_int_distribution<Int> dist(l, r);
  return dist(rng);
}

template <std::random_access_iterator Iter, typename Int = std::iter_value_t<Iter>>
requires std::integral<Int>
void random_permutation(Iter l, Iter r, Int base = 1) {
  std::iota(l, r, base);
  std::shuffle(l, r, rng);
}

template <std::forward_iterator Iter, typename Int = std::iter_value_t<Iter>>
requires std::integral<Int>
void random_array(Iter l, Iter r, Int lo, Int hi) {
  std::for_each(l, r, [lo, hi](Int& x) {
    x = randint(lo, hi);
  });
}

/// `E[height(T)] = O(log n)`
inline auto random_tree_random_father(int n) -> std::vector<std::array<int, 2>> {
  std::vector<std::array<int, 2>> edges(n - 1);
  for (int i = 2; i < n; ++i) {
    edges[i - 2] = {randint(1, i - 1), i};
  }
  return edges;
}

/// `E[height(T)] = O(sqrt(n))`
// inline auto random_tree_random_prufer(int n) -> std::vector<std::array<int, 2>> {
//   if (n == 1) return {};
//   if (n == 2) return {{1, 2}};
//   std::vector<int> prufer(n);
//   random_permutation(prufer.begin(), prufer.end());
//   return prufer_to_tree(prufer.begin(), prufer.begin() + n - 2);
// }
} // namespace nnk
