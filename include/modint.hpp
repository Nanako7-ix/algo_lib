#pragma once
#include <type_traits>

namespace nnk {
template <typename T>
struct is_modint : std::false_type {};

template <typename T>
concept modint = is_modint<T>::value;
} // namespace nnk
