#pragma once

#include <type_traits>
#include <vector>
#include <string>
#include <tuple>

struct has_smth {
protected:
    typedef char yes;
    typedef short no;
};

template <int N> struct count_saver { short a; static const int value = N; };
template <typename T> struct type_saver { short a; using type = T; };

template <typename T, int N> struct array_decl { static T a[N]; using type = decltype(a); };

template<typename T>
struct has_types_as_tuple
{
private:
    template<typename C> static count_saver<std::tuple_size<typename C::types_as_tuple>::value> test(typename C::types_as_tuple*);
    template<typename C> static count_saver<0> test(...);
public:
    using result_type = decltype(test<T>(0));
    static const bool value = result_type::value > 0;
    static const int value_count = result_type::value;
    typedef T type;
};

//template<typename T, int N>
//struct has_mem_names
//{
//    using no = char;
//    using yes = short;
//private:
//    template<typename C> static yes test(decltype(C::member_count)*);
//    template<typename C> static no test(...);
//public:
//    using result_type = decltype(test<T>(0));
//    static const bool value = sizeof(test<T>(0)) == sizeof(yes);//std::is_same<typename result_type::type, typename array_decl<const char*, N>::type>::value;
//    typedef T type;
//};

template<typename T>
struct is_structure : std::integral_constant<bool, std::is_class<T>::value
//                                                && has_mem_names<T, has_types_as_tuple<T>::value_count>::value
                                                && has_types_as_tuple<T>::value>
{};


template<typename T>
struct has_const_iterator : has_smth
{
private:
    template<typename C> static yes test(typename C::const_iterator*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template <typename T>
struct has_begin_end
{
    template<typename C> static char (&f(typename std::enable_if<
        std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
        typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&f(...))[2];

    template<typename C> static char (&g(typename std::enable_if<
        std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
        typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&g(...))[2];

    static bool const beg_value = sizeof(f<T>(0)) == 1;
    static bool const end_value = sizeof(g<T>(0)) == 1;
};

template<typename T>
struct is_container : std::integral_constant<bool, has_const_iterator<T>::value
                                                && has_begin_end<T>::beg_value
                                                && has_begin_end<T>::end_value>
{};

template<typename T> struct is_string    { constexpr static const bool value = false; };

template<> struct is_string<std::string>      { constexpr static const bool value = true; };
//template<> struct is_string<std::string_view> { constexpr static const bool value = true; };
//template<> struct is_string<QString>          { constexpr static const bool value = true; };
//template<> struct is_string<QStringRef>       { constexpr static const bool value = true; };
//template<> struct is_string<QStringView>      { constexpr static const bool value = true; };

template<typename T>
struct is_nostring_container {
    constexpr static const bool value = is_container<T>::value && !is_string<T>::value;
};

template<typename T>
struct has_first_type : has_smth
{
private:
    template<typename C> static yes test(typename C::first_type*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template<typename T>
struct has_second_type : has_smth
{
private:
    template<typename C> static yes test(typename C::second_type*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template<typename T>
struct has_key_type : has_smth
{
private:
    template<typename C> static yes test(typename C::key_type*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template<typename T>
struct has_mapped_type : has_smth
{
private:
    template<typename C> static yes test(typename C::mapped_type*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template<typename T>
struct has_value_type : has_smth
{
private:
    template<typename C> static yes test(typename C::value_type*);
    template<typename C> static no  test(...);
public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template <typename T>
struct is_pair {
    constexpr static bool value = has_first_type<T>::value && has_second_type<T>::value;
};

template <typename T>
struct is_map {
    constexpr static bool value = is_container<T>::value && has_key_type<T>::value && has_mapped_type<T>::value;
};


//struct
