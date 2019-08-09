#pragma once

#include "type_group.hpp"
#include "metafunction.hpp"
#include "detail/metadata.hpp"
#include <functional>
#include <iostream>
#include <map>

/**
 * @brief description what's gonna be added within DECL_METAINFO(Class, Args...) (pseudocode)
 // types
     using types_as_tuple = std::tuple<Args...>;
     using member_type<N> = types_as_tuple<N>;
     enum class e { invalid = -1, Args... };
 // members
 static const size_t member_count;
 // methods
     static const char* get_classname(); // #Class
     static const char* get_declare_string(); // #Args...
     static const char* member_name(int index);
     static int member_index(const char* name);
     static offset_type member_offset(int index);

     offset_type member_address(int index) const;
     const member_type<N>& get_member<N>(int index) const;
           member_type<N>& get_member<N>(int index);
     int member_index(const T& mem) const;

     difference_type<e>           get_not_equal_members(const Class& other) const;
     difference_type<const char*> get_not_equal_member_names(const Class& other) const;
 // operators
    bool operator==(const Class& other);
    bool operator!=(const Class& other);
 //...
 */

namespace
{
    template <typename C, typename R>
    R get_method_return_type(R(C::*)()const);
}//namespace

#define DECL_METAINFO(classname, ...) \
public:\
    CONSTEXPR static const char* get_classname() { return #classname; }\
    CONSTEXPR static const char* get_declare_string() { return #__VA_ARGS__; }\
    auto get_members_as_tuple() const -> decltype(std::make_tuple(__VA_ARGS__));/* { return std::make_tuple(__VA_ARGS__); }*/\
    using types_as_tuple = decltype(::get_method_return_type<classname>(&classname::get_members_as_tuple));\
    enum class e : int\
    {\
        invalid = -1,\
        __VA_ARGS__\
    };\
    CONSTEXPR static const size_t member_count = std::tuple_size<types_as_tuple>::value;\
    inline metafunction::offset_array_type<member_count> _get_offsets() const {\
        return metafunction::detail::get_offsets<member_count>(*this, __VA_ARGS__);\
    }\
    inline static metafunction::offset_array_type<member_count> get_offsets() {\
        return get_metadata().offsets;\
    }\
    template <int N> using member_type = typename std::tuple_element<(N + member_count) % member_count, types_as_tuple>::type;\
    inline static std::string member_name(int i) {\
        return get_metadata().names.at((i + member_count) % member_count).tostring();\
    }\
    inline static int member_index(const char* member_name) {\
        return get_metadata().member_index(member_name);\
    }\
    inline static metafunction::offset_type member_offset(int i) {\
        return get_offsets().at((i + member_count) % member_count);\
    }\
    inline metafunction::address_type member_address(int i) const {\
        return metafunction::detail::address(*this) + member_offset(i);\
    }\
    template <int N> inline const member_type<N>& get_member() const {\
        return *(reinterpret_cast<const member_type<N>*>(member_address(N)));\
    }\
    template <int N> inline member_type<N>& get_member() {\
        return *((member_type<N>*)member_address(N));\
    }\
    inline bool operator!=(const classname& other) const { return metafunction::is_different(*this, other); }\
    inline bool operator==(const classname& other) const { return !operator!=(other); }\
    inline metafunction::difference_type<e> get_not_equal_members(const classname& other) const {\
        return metafunction::get_difference(*this, other);\
    }\
    inline metafunction::difference_type<std::string> get_not_equal_member_names(const classname& other) const {\
        metafunction::difference_type<std::string> lst;\
        for (auto m: get_not_equal_members(other)) {\
            lst.insert(member_name(int(m)));\
        }\
        return lst;\
    }\
    template <typename T>\
    inline int member_index(const T& member) const {\
        return get_metadata().member_index(metafunction::detail::address(member) - metafunction::detail::address(*this));\
    }\
    static const metafunction::detail::metadata<classname>& get_metadata() {\
        using namespace metafunction::detail;\
        static const classname obj;\
        static const auto _metadata = metadata<classname>(wrap_to_array<member_count>(#__VA_ARGS__)\
                                                        , obj._get_offsets());\
        return _metadata;\
    }

