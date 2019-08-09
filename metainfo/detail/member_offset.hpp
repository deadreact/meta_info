#pragma once

#include <inttypes.h>
#include <deque>
#include <array>

namespace metafunction {
/** -------------------------------------------------------------------------------- */
using address_type = const uint8_t*;
using offset_type = decltype(address_type() - address_type());
using offset_list_type = std::deque<offset_type>;
template <size_t N> using offset_array_type = std::array<offset_type, N>;
/** -------------------------------------------------------------------------------- */
namespace detail {
template <typename T>
address_type address(const T& arg);

template <int N, typename Cls, typename T, typename ...Args>
offset_array_type<N> get_offsets(const Cls &obj, const T &arg, const Args &...args);

template <int N, typename Cls, typename T>
offset_array_type<N> get_offsets(const Cls& obj, const T& arg);

/** -------------------------------------------------------------------------------- */
} //namespace detail
} //namespace metafunction

template <typename T>
metafunction::address_type metafunction::detail::address(const T& arg) {
    return reinterpret_cast<address_type>(std::addressof(arg));
}

template<int N, typename Cls, typename T, typename... Args>
metafunction::offset_array_type<N> metafunction::detail::get_offsets(const Cls &obj, const T &arg, const Args &... args) {
    offset_array_type<N> arr = get_offsets<N>(obj, args...);
    arr[N - sizeof...(args) - 1] = address(arg) - address(obj);
    return arr;
}

template<int N, typename Cls, typename T>
metafunction::offset_array_type<N> metafunction::detail::get_offsets(const Cls& obj, const T &arg) {
    offset_array_type<N> arr;
    arr[N - 1] = address(arg) - address(obj);
    return arr;
}

