#pragma once

#include "detail/member_count.hpp"
#include "detail/member_offset.hpp"
#include "detail/member_name.hpp"
#include "detail/json_converter.hpp"
#include "detail/compare.hpp"

namespace metafunction
{
/** ------------------------------------------------------------------------------- */
template <typename T>
inline size_t members_count(const T* obj) {
    return detail::subelements_count_obj<T>::get_value(obj);
}

template <typename T>
inline std::string to_json_string(const T& obj) {
    return detail::json_converter<T>::to_json_string(obj);
}

template <typename T>
inline bool is_different(const T& obj1, const T& obj2) {
    return !detail::compare_helper<T>::cmp1(obj1, obj2);
}

template <typename T>
inline difference_type get_difference(const T& obj1, const T& obj2) {
    difference_type diff;
    detail::compare_helper<T>::cmp2(diff, obj1, obj2);
    return diff;
}
/** ------------------------------------------------------------------------------- */
} //namespace metafunction
