#pragma once

#include "type_group.hpp"
#include "metafunction.hpp"
#include <functional>
#include <iostream>
#include <map>

#define  DECL_TYPES_AS_TUPLE(classname, ...) \
private:\
    struct _detail {\
        constexpr static const char* member_names = #__VA_ARGS__;\
    };\
public:\
    using types_as_tuple = decltype(std::make_tuple(__VA_ARGS__));\
    template <int N> using member_type = typename std::tuple_element<N, types_as_tuple>::type;\
    inline static const char* member_name(int i) {\
        static const std::vector<std::string> names = metafunction::detail::wrap_members(_detail::member_names);\
        return names.at(i).c_str();\
    }\
    inline metafunction::offset_list_type _get_offsets() const {\
        return metafunction::detail::get_offsets(*this, __VA_ARGS__);\
    }\
    inline static metafunction::offset_list_type get_offsets() {\
        static const classname obj;\
        return obj._get_offsets();\
    }\
    inline static metafunction::offset_type member_offset(int i) {\
        return get_offsets().at(i);\
    }\
    inline static metafunction::address_type member_address(const classname& obj, int i) {\
        return metafunction::detail::address(obj) + member_offset(i);\
    }\
    template <int N> inline const member_type<N>& get_member() const {\
        return *reinterpret_cast<const member_type<N>*>(member_address(*this, N));\
    }\
    template <int N> inline member_type<N>& get_member() {\
        return *((member_type<N>*)(member_address(*this, N)));\
    }\
    inline bool operator!=(const classname& other) const { return metafunction::is_different(*this, other); }\
    inline bool operator==(const classname& other) const { return !operator!=(other); }\
    inline metafunction::difference_type get_not_equal_member_indices(const classname& other) const {\
        return metafunction::get_difference(*this, other);\
    }\
    inline std::set<const char*> get_not_equal_member_names(const classname& other) const {\
        std::set<const char*> lst;\
        for (int index: get_not_equal_member_indices(other)) {\
            lst.insert(member_name(index));\
        }\
        return lst;\
    }
