#pragma once

#include <type_traits>
#include <vector>
#include <string>
#include <tuple>
#include "metaconfig.hpp"

/**
 * @name SFINAE_INTERNAL_TYPE
 * @brief used to declare a mechanism to test classes for internal type existence in compile time
 * @example:
 * // To test existence of underlying container:
 * SFINAE_INTERNAL_TYPE(container_type);
 * has_container_type<std::stack<int>>::value // true, "std::stack<int>::container_type" fragment is valid
 * has_container_type<std::vector<int>>::value // false, "std::vector<int>::container_type" fragment is invalid
 *
 * struct MyStruct { using container_type = std::list<short>; };
 * has_container_type<MyStruct>::value // true, "MyStruct::container_type" fragment is valid
 */
#define SFINAE_INTERNAL_TYPE(_type) \
template<typename T>\
struct has_##_type##_impl {\
    template<typename _Tp, typename = typename _Tp::_type> static std::true_type  test(int);\
    template<typename>                                     static std::false_type test(...);\
    typedef decltype(test<T>(0)) type;\
};\
template <typename T> struct has_##_type: has_##_type##_impl<T>::type {}

SFINAE_INTERNAL_TYPE(types_as_tuple);
SFINAE_INTERNAL_TYPE(const_iterator);
SFINAE_INTERNAL_TYPE(first_type);
SFINAE_INTERNAL_TYPE(second_type);
SFINAE_INTERNAL_TYPE(value_type);
SFINAE_INTERNAL_TYPE(key_type);
SFINAE_INTERNAL_TYPE(mapped_type);
SFINAE_INTERNAL_TYPE(decorated_type);

/**
 * @class has_begin_end
 * @brief used to test classes for begin() && end() methods existence, requires constexpr support by compiler
 * implementation specific taken from 'type_traits', std::is_destructible (gcc)
 */

struct __do_has_begin_end
{
    template<typename _Tp, typename = decltype(std::declval<_Tp&>().begin())>
    static std::true_type test_begin(int);
    template<typename _Tp, typename = decltype(std::declval<_Tp&>().end())>
    static std::true_type test_end(int);

    template<typename> static std::false_type test_begin(...);
    template<typename> static std::false_type test_end(...);
};

template<typename _Tp>
struct __has_begin_end_impl: public __do_has_begin_end
{
    typedef decltype(test_begin<_Tp>(0)) begin_type;
    typedef decltype(test_end<_Tp>(0))   end_type;
    using type = typename std::conditional<std::is_same<begin_type, end_type>::value, begin_type, std::false_type>::type;
};

template<typename _Tp>
struct has_begin_end: __has_begin_end_impl<_Tp>::type {};

template<typename T> struct is_structure : std::integral_constant<bool, std::is_class<T>::value && has_types_as_tuple<T>::value> {};
template<typename T> struct is_container : std::integral_constant<bool, has_const_iterator<T>::value && has_begin_end<T>::value> {};
template<typename T> struct is_big_int   : std::integral_constant<bool, std::is_integral<T>::value && (sizeof(T) > sizeof(int))> {};
template<typename T> struct is_string    : std::integral_constant<bool, std::is_same<T, std::string>::value> {};
template<typename T> struct is_pair      : std::integral_constant<bool, has_first_type<T>::value && has_second_type<T>::value> {};
template<typename T> struct is_map       : std::integral_constant<bool, is_container<T>::value && has_key_type<T>::value && has_mapped_type<T>::value> {};
template<typename T> struct is_nostring_container : std::integral_constant<bool, is_container<T>::value && !is_string<T>::value> {};

template <typename T, bool = has_decorated_type<T>::value>
struct remove_decorator_type {
    using type = T;
};

template <typename T>
struct remove_decorator_type<T, true> {
    using type = typename remove_decorator_type<typename T::decorated_type>::type;
};
