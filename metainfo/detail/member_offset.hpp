#pragma once

#include <inttypes.h>
#include <deque>

namespace metafunction {
/** -------------------------------------------------------------------------------- */
using address_type = const uint8_t*;
using offset_type = decltype(address_type() - address_type());
using offset_list_type = std::deque<offset_type>;
/** -------------------------------------------------------------------------------- */
namespace detail {
template <typename T>
address_type address(const T& arg);

template <typename Cls, typename T, typename ...Args>
offset_list_type get_offsets(const Cls& cls, const T& arg, const Args& ...args);

template <typename Cls, typename T>
offset_list_type get_offsets(const Cls& cls, const T& arg);

/** -------------------------------------------------------------------------------- */
} //namespace detail
} //namespace metafunction

template <typename T>
metafunction::address_type metafunction::detail::address(const T& arg) {
    return reinterpret_cast<address_type>(std::addressof(arg));
}

template <typename Cls, typename T, typename ...Args>
metafunction::offset_list_type metafunction::detail::get_offsets(const Cls& cls, const T& arg, const Args& ...args) {
    offset_list_type lst = get_offsets(cls, args...);
    lst.push_front(address(arg) - address(cls));
    return lst;
}

template <typename Cls, typename T>
metafunction::offset_list_type metafunction::detail::get_offsets(const Cls& cls, const T& arg) {
    return offset_list_type({address(arg) - address(cls)});
}

