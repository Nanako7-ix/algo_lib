#pragma once
#include <array>
#include <cassert>
#include <cstddef>
#include <vector>

namespace nnk {
template <std::size_t N>
class suffix_automaton {
public:
  constexpr suffix_automaton() : cnt(0), sam(2) {}

  constexpr void reserve(std::size_t n) {
    sam.reserve(n);
  }

  constexpr int extend(int p, int ch) {
    int u = new_node();
    sam[u].len = sam[p].len + 1;

    for (; p && next(p, ch) == 0; p = link(p)) {
      sam[p].next[ch] = u;
    }

    if (p == 0) {
      sam[u].link = 1;
    } else {
      int q = next(p, ch);
      if (sam[p].len + 1 == len(q)) {
        sam[u].link = q;
      } else {
        int r = new_node();

        sam[r].len = len(p) + 1;
        sam[r].link = link(q);
        sam[r].next = next(q);
        sam[q].link = sam[u].link = r;
        for (; p && next(p, ch) == q; p = link(p)) {
          sam[p].next[ch] = r;
        }
      }
    }
    cnt += len(u) - len(link(u));
    return u;
  }

  static constexpr std::size_t sigma() noexcept {
    return N;
  }

  constexpr std::size_t size() const noexcept {
    return sam.size() - 1;
  }

  constexpr std::size_t count() const noexcept {
    return cnt;
  }

  constexpr int len(int u) const noexcept {
    return sam[u].len;
  }
  constexpr int link(int u) const noexcept {
    return sam[u].link;
  }
  constexpr int next(int u, int ch) const noexcept {
    return sam[u].next[ch];
  }
  constexpr std::array<int, N> next(int u) const noexcept {
    return sam[u].next;
  }

  constexpr auto get_tree() const -> std::vector<std::vector<int>> {
    std::size_t sz = size();

    std::vector<std::vector<int>> adj(sz + 1);
    for (std::size_t u = 2; u <= sz; ++u) {
      adj[link(u)].push_back(u);
    }
    return adj;
  }

private:
  struct node {
    int len = 0, link = 0;
    std::array<int, N> next{};
  };

  std::size_t cnt = 0;
  std::vector<node> sam;

  constexpr int new_node() {
    sam.emplace_back();
    return sam.size() - 1;
  }
};
} // namespace nnk
