#pragma once

#include "../type_group.hpp"

namespace metafunction {
namespace detail {
/** ------------------------------------------------------------------------------- */
template <typename T, metafunction::e_type_group = metafunction::type_group_detect<T>::type_group>
struct subelements_count_obj {
    constexpr static size_t get_value(const T* = nullptr) { return 0; }
};
template <typename T>
struct subelements_count_obj<T, metafunction::e_type_group::pair> {
    static const size_t value = 2;
    constexpr static size_t get_value(const T* = nullptr) { return value; }
};
template <typename T>
struct subelements_count_obj<T, metafunction::e_type_group::structure> {
    static const size_t value = std::tuple_size<typename T::types_as_tuple>::value;
    constexpr static size_t get_value(const T* = nullptr) { return value; }
};
template <typename T>
struct subelements_count_obj<T, metafunction::e_type_group::list> {
    constexpr static size_t get_value(const T* obj = nullptr) { return obj ? obj->size(): 0; }
};

/** ------------------------------------------------------------------------------- */
} //namespace detail
} //namespace metafunction
