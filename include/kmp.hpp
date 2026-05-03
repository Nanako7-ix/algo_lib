#pragma once
#include <concepts>
#include <iterator>
#include <vector>

namespace nnk {
template <std::random_access_iterator Iter>
auto prefix_function(Iter first, Iter last) -> std::vector<int> {
  int n = static_cast<int>(std::distance(first, last));
  std::vector<int> fail(n + 1);
  for (int i = 1, j = 0; i < n; ++i) {
    while (j && *(first + i) != *(first + j)) j = fail[j];
    if (*(first + i) == *(first + j)) ++j;
    fail[i + 1] = j;
  }
  return fail;
}

template <typename T>
struct prefix_function_builder {
public:
  prefix_function_builder() : m_s{}, m_fail{0} {};

  template <std::random_access_iterator Iter>
  prefix_function_builder(Iter first, Iter last)
      : m_s(first, last),
        m_fail(prefix_function(first, last)) {}

  void push(T ch) {
    m_s.push_back(ch);
    m_fail.push_back(m_fail.back());

    // 特判 fail[1] = 0
    if (m_s.size() == 1) return;

    int& j = m_fail.back();
    while (j && ch != m_s[j]) j = m_fail[j];
    j += ch == m_s[j];
  }

  void pop() {
    m_s.pop_back();
    m_fail.pop_back();
  }

  int operator[](std::size_t p) const {
    return m_fail[p];
  }

  const std::vector<T>& string() const { return m_s; }
  const std::vector<int>& fail() const { return m_fail; }
  const std::size_t size() const { return m_s.size(); }

private:
  std::vector<T> m_s;
  std::vector<int> m_fail;
};

template <std::random_access_iterator Iter>
prefix_function_builder(Iter, Iter) -> prefix_function_builder<std::iter_value_t<Iter>>;

template <std::random_access_iterator Iter1, std::random_access_iterator Iter2>
requires std::equality_comparable_with<std::iter_value_t<Iter1>, std::iter_value_t<Iter2>>
auto occurrence(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2) -> std::vector<int> {
  auto fail = prefix_function(first2, last2);

  std::size_t n = std::distance(first1, last1);
  std::size_t m = std::distance(first2, last2);

  if (n == 0 || m == 0) return {};  

  std::vector<int> occur;
  for (std::size_t i = 0, j = 0; i < n; ++i) {
    while (j && *(first1 + i) != *(first2 + j)) j = fail[j];
    if (*(first1 + i) == *(first2 + j)) ++j;
    if (j == m) {
      occur.push_back(i - m + 1);
      j = fail[j];
    }
  }
  return occur;
}
} // namespace nnk
