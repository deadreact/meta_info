#pragma once

#include "member_count.hpp"
#include <sstream>
#include <map>


namespace metafunction {
namespace detail {
/** ------------------------------------------------------------------------------ */
template <typename T, metafunction::e_type_group = metafunction::type_group_detect<T>::type_group>
struct json_converter;

template <typename T, int N = subelements_count_obj<T>::value - 1>
struct json_converter_helper {
    static void members_to_json_string(std::map<std::string, std::string>& dict, const T& obj) {
        dict.insert(std::pair<std::string, std::string>(T::member_name(N), json_converter<typename T::template member_type<N>>::to_json_string(obj.template get_member<N>())));
        json_converter_helper<T, N-1>::members_to_json_string(dict, obj);
    }
};

template <typename T>
struct json_converter_helper<T, 0> {
    static void members_to_json_string(std::map<std::string, std::string>& dict, const T& obj) {
        dict.insert(std::pair<std::string, std::string>(T::member_name(0), json_converter<typename T::template member_type<0>>::to_json_string(obj.template get_member<0>())));
    }
};

template <typename T>
struct json_converter<T, metafunction::e_type_group::pair> {
    static std::string to_json_string(const T& pair)
    {
        std::ostringstream ss;
        ss << "{\"first\": " << pair.first << ",\"second\": " << pair.second << "}";
        return ss.str();
    }
};

template <typename T>
struct json_converter<T, metafunction::e_type_group::list> {
    static std::string to_json_string(const T& lst)
    {
        if (lst.empty()) {
            return "[]";
        }
        std::ostringstream ss;
        auto it = std::begin(lst);
        ss << "[" << json_converter<typename T::value_type>::to_json_string(*it);
        ++it;
        for (;it != std::end(lst); ++it) {
            ss << "," << json_converter<typename T::value_type>::to_json_string(*it);
        }
        ss << "]";
        return ss.str();
    }
};

template <typename T>
struct json_converter<T, metafunction::e_type_group::single> {
    static std::string to_json_string(const T& arg)
    {
        std::ostringstream ss;
        ss << arg;
        return ss.str();
    }
};

template <>
struct json_converter<std::string> {
    static std::string to_json_string(const std::string& arg)
    {
        std::ostringstream ss;
        ss << "\"" << arg << "\"";
        return ss.str();
    }
};

template <>
struct json_converter<const char*> {
    static std::string to_json_string(const char* arg)
    {
        std::ostringstream ss;
        ss << "\"" << arg << "\"";
        return ss.str();
    }
};

template <typename T>
struct json_converter<T, metafunction::e_type_group::enumerator> {
    static std::string to_json_string(T arg)
    {
        std::ostringstream ss;
        ss << int(arg);
        return ss.str();
    }
};

template <typename T>
struct json_converter<T, metafunction::e_type_group::structure> {
    static std::string to_json_string(const T& obj)
    {
        if (subelements_count_obj<T>::get_value(&obj) == 0) {
            return "{}";
        }
        std::map<std::string, std::string> dict;
        json_converter_helper<T>::members_to_json_string(dict, obj);
        std::ostringstream ss;
        ss << "{\"" << obj.member_name(0) << "\": " << dict.at(obj.member_name(0));
        for (size_t i = 1; i < subelements_count_obj<T>::get_value(&obj); i++) {
            ss << ",\"" << obj.member_name(i) << "\": " << dict.at(obj.member_name(i));
        }
        ss << "}";
        return ss.str();
    }
};
/** ------------------------------------------------------------------------------ */
} //namespace detail
} //namespace metafunction
