#pragma once

#include "detail/member_count.hpp"
#include "detail/member_offset.hpp"
#include "detail/member_name.hpp"
#include "detail/json_converter.hpp"
#include "detail/compare.hpp"

Json::Value jsonFromString(const std::string& str);

namespace metafunction
{
/** ------------------------------------------------------------------------------- */
template <typename T>
inline size_t members_count(const T& obj) {
    return detail::subelements_count_obj<T>::get_value(&obj);
}

template <typename T>
inline std::string to_json_string(const T& obj) {
    return detail::json_converter<T>::to_json_string(obj);
}
    
template <typename T>
inline Json::Value to_json(const T& obj) {
    return jsonFromString(to_json_string(obj));
}

template <typename T>
inline bool from_json(const Json::Value& in, T& obj) {
    return detail::json_converter<T>::from_json(in, obj);
}

template <typename T>
inline bool from_json_string(const std::string& in, T& obj) {
    return from_json(jsonFromString(in), obj);
}

template <typename T>
inline bool is_different(const T& obj1, const T& obj2) {
    return !detail::compare_helper<T>::cmp1(obj1, obj2);
}

template <typename T>
inline difference_type<typename T::e> get_difference(const T& obj1, const T& obj2) {
    difference_type<typename T::e> diff;
    detail::compare_helper<T>::cmp2(diff, obj1, obj2);
    return diff;
}

/** ------------------------------------------------------------------------------- */
} //namespace metafunction
