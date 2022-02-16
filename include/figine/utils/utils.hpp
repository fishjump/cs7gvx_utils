#pragma once

#include "figine/dep.hpp"

#include <type_traits>

#define __MERGE(x, y) x_##y
#define _MERGE(x, y) __MERGE(x, y)
#define UNIQUE(x) _MERGE(x, __COUNTER__)

#define defer(expr) figine::utils::_defer_t UNIQUE(_defer) = [&]() { expr; }

namespace figine::utils {

template <class F> struct _defer_t {
  F func;
  _defer_t(F f) : func(f) {}
  ~_defer_t() { func(); }
};

template <class> inline constexpr bool dependent_false_v = false;

template <class T, int _sz> struct is_glm_vec : public std::false_type {};
template <class T, int _sz, glm::qualifier Q>
struct is_glm_vec<glm::vec<_sz, T, Q>, _sz> : public std::true_type {};
template <class T, int _sz>
inline constexpr bool is_glm_vec_v = is_glm_vec<T, _sz>::value;

template <class T, int _sz> struct is_glm_mat : public std::false_type {};
template <class T, int _sz, glm::qualifier Q>
struct is_glm_mat<glm::mat<_sz, _sz, T, Q>, _sz> : public std::true_type {};
template <class T, int _sz>
inline constexpr bool is_glm_mat_v = is_glm_mat<T, _sz>::value;

template <class T, class U> inline size_t offset_diff(const T &t, const U &u) {
  return ((size_t)(&t) - (size_t)(&u));
}

template <class T>
using value_type_t = std::decay_t<decltype(std::declval<T>()[0])>;

} // namespace figine::utils
