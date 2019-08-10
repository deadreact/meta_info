#pragma once

#include "member_count.hpp"
#include <sstream>
#include <map>
#include <jsoncpp/json.h>
#include <functional>
#include <set>
#include <list>
#include <array>
#include <limits>
namespace metafunction {
namespace detail {
/** ------------------------------------------------------------------------------ */

template <typename _Tp, bool = std::is_enum<_Tp>::value>
struct enum_unpacker { using type = _Tp; };

template <typename _Tp>
struct enum_unpacker<_Tp, true> { using type = typename std::underlying_type<_Tp>::type; };

/**
 * @name json_value_detect
 * @brief guess Json::ValueType from type T, used in from_json functions
 */
template <typename _Tp, bool is_decorator = has_decorated_type<_Tp>::value>
struct json_value_detect {
    using T = typename enum_unpacker<_Tp>::type;
    static const Json::ValueType type =
            std::is_same<T, std::string>::value || std::is_same<T, char>::value
            ? Json::stringValue
            : std::is_arithmetic<T>::value
              ? std::is_floating_point<T>::value || is_big_int<T>::value
                ? Json::realValue
                : std::is_unsigned<T>::value
                  ? Json::uintValue
                  : Json::intValue
              : std::is_same<T, bool>::value
                ? Json::booleanValue
                : is_structure<T>::value
                  ? Json::objectValue
                  : is_container<T>::value
                    ? Json::arrayValue
                    : Json::nullValue;
};

template <typename _Tp>
struct json_value_detect<_Tp, true> {
    static const Json::ValueType type = json_value_detect<typename _Tp::decorated_type>::type;
};

/**
 * @name container_append
 * @brief function appends a value to container, overloaded for the most usable stl containers
 */
template <typename... Args>
void container_append(std::vector<Args...>& container, typename std::vector<Args...>::value_type&& val, int) {
    container.push_back(std::move(val));
}
template <typename... Args>
void container_append(std::deque<Args...>& container, typename std::deque<Args...>::value_type&& val, int) {
    container.push_back(std::move(val));
}
template <typename... Args>
void container_append(std::list<Args...>& container, typename std::list<Args...>::value_type&& val, int) {
    container.push_back(std::move(val));
}
template <typename... Args>
void container_append(std::set<Args...>& container, typename std::set<Args...>::value_type&& val, int) {
    container.insert(std::move(val));
}
template <typename... Args>
void container_append(std::map<Args...>& container, typename std::map<Args...>::value_type&& val, int) {
    container.insert(std::move(val));
}

template <typename T, size_t N>
void container_append(std::array<T, N>& container, T&& val, int index) {
//    DEBUG_ASSERT(index < N);
    if (index < N) {
        container[index] = std::move(val);
    }
}

/**
 * @struct from_json_helper
 * @brief used to convert and write json values to primitive type vars like int, float...
 */

template <typename T, Json::ValueType jsonType = json_value_detect<T>::type>
struct from_json_helper;

template <typename T>
struct from_json_helper<T, Json::intValue> {
    inline static bool from_json(const Json::Value& jsonVal, T& val) {
        using type = typename remove_decorator_type<T>::type;
        int intVal = jsonVal.asInt();
        if (intVal >= std::numeric_limits<type>::min() && intVal <= std::numeric_limits<type>::max()) {
            val = static_cast<T>(intVal);
            return true;
        }
        return false; // overflow
    }
};
template <typename T>
struct from_json_helper<T, Json::uintValue> {
    inline static bool from_json(const Json::Value& jsonVal, T& val) {
        using type = typename remove_decorator_type<T>::type;
        unsigned int uintVal = jsonVal.asUInt();
        if (uintVal >= std::numeric_limits<type>::min() && uintVal <= std::numeric_limits<type>::max()) {
            val = static_cast<T>(uintVal);
            return true;
        }
        return false; // overflow
    }
};
template <typename T>
struct from_json_helper<T, Json::realValue> {
    inline static bool from_json(const Json::Value& jsonVal, T& val) {
        val = jsonVal.asDouble();
        return true;
    }
};
template <typename T>
struct from_json_helper<T, Json::stringValue> {
    inline static bool from_json(const Json::Value& jsonVal, T& val) {
        val = jsonVal.asString();
        return true;
    }
};
template <>
struct from_json_helper<char> {
    inline static bool from_json(const Json::Value& jsonVal, char& val) {
        std::string str = jsonVal.asString();
        if (str.size() == 1) {
            val = str.at(0);
            return true;
        }
        return false;
    }
};

/**
 * @struct json_converter
 * @brief creates common generic interface to convert and read/write Json::Value <--> structs/containers
 */
template <typename T, metafunction::e_type_group = metafunction::type_group_detect<T>::type_group>
struct json_converter;

template <typename T, int N = subelements_count_obj<T>::value - 1>
struct json_converter_helper {
    static void members_to_json_string(std::map<std::string, std::string>& dict, const T& obj) {
        dict.insert(std::pair<std::string, std::string>(T::member_name(N), json_converter<typename T::template member_type<N>>::to_json_string(std::get<N>(obj))));
        json_converter_helper<T, N-1>::members_to_json_string(dict, obj);
    }

    static bool members_from_json(const Json::Value& in, T& obj)
    {
#ifdef CUT_m_FROM_NAMES
        static const std::string name = in.isMember(std::string("m_") + T::member_name(N)) ? std::string("m_") + T::member_name(N) : T::member_name(N);
#else
        static const std::string name = T::member_name(N);
#endif //CUT_m_FROM_NAMES
        if (in.isMember(name))
        {
            return json_converter<typename T::template member_type<N>>::from_json(in[name], std::get<N>(obj))
                && json_converter_helper<T, N-1>::members_from_json(in, obj);
        }
        return json_converter_helper<T, N-1>::members_from_json(in, obj);
    }
};

template <typename T>
struct json_converter_helper<T, 0> {
    static void members_to_json_string(std::map<std::string, std::string>& dict, const T& obj) {
        dict.insert(std::pair<std::string, std::string>(T::member_name(0), json_converter<typename T::template member_type<0>>::to_json_string(std::get<0>(obj))));
    }
    static bool members_from_json(const Json::Value& in, T& obj)
    {
#ifdef CUT_m_FROM_NAMES
        static const std::string name = in.isMember(std::string("m_") + T::member_name(0)) ? std::string("m_") + T::member_name(0) : T::member_name(0);
#else
        static const std::string name = T::member_name(0);
#endif //CUT_m_FROM_NAMES
        if (in.isMember(name))
        {
            return json_converter<typename T::template member_type<0>>::from_json(in[name], std::get<0>(obj));
        }
        return true;
    }
};

template <typename T>
struct json_converter<T, metafunction::e_type_group::pair> {
    static std::string to_json_string(const T& pair)
    {
        std::ostringstream ss;
        ss << "{\"first\": "  << json_converter<typename T::first_type>::to_json_string(pair.first)
           << ",\"second\": " << json_converter<typename T::second_type>::to_json_string(pair.second) << "}";
        return ss.str();
    }
    static bool from_json(const Json::Value& jsonValue, T& pair)
    {
        if (jsonValue.isObject() && jsonValue.isMember("first") && jsonValue.isMember("second")) {
            return json_converter<typename T::first_type>::from_json(jsonValue["first"], pair.first)
                && json_converter<typename T::second_type>::from_json(jsonValue["second"], pair.second);
        }
        return false;
    }
};

template <typename T>
struct json_converter<T, metafunction::e_type_group::container> {
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

    static bool from_json(const Json::Value& jsonValue, T& lst)
    {
        if (jsonValue.isArray())
        {
            using value_type = typename T::value_type;

            T lstTmp;
            // if this is an std::array
            if (!lstTmp.empty()) {
                lstTmp = lst;
            }

            for (int index = 0; index < jsonValue.size(); index++)
            {
                const Json::Value& jVal = jsonValue[index];
                value_type value;
                if (!json_converter<value_type>::from_json(jVal, value)) {
                    return false;
                }
                detail::container_append(lstTmp, std::move(value), index);
            }
            std::swap(lst, lstTmp);
            return true;
        }
        return false;
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
    static bool from_json(const Json::Value& jsonValue, T& arg)
    {
        if (jsonValue.isConvertibleTo(json_value_detect<T>::type))
        {
            from_json_helper<T>::from_json(jsonValue, arg);
            return true;
        }
        return false;
    }
};

template <>
struct json_converter<bool> {
    static std::string to_json_string(bool arg)
    {
        return arg ? "true" : "false";
    }
    static bool from_json(const Json::Value& jsonValue, bool& arg)
    {
        if (jsonValue.isNull()) {
            arg = false;
            return true;
        }
        if (jsonValue.isBool() || (jsonValue.isIntegral() && jsonValue.asUInt() <= 1))
        {
            arg = jsonValue.asBool();
            return true;
        }
        return false;
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
    static bool from_json(const Json::Value& jsonValue, std::string& arg)
    {
        if (jsonValue.isNull()) {
            arg.clear(); // arg = "null" ??
            return true;
        }
        if (jsonValue.isString()) {
            arg = jsonValue.asString();
            return true;
        }
        return false;
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

// signed char should be converted to number
template <>
struct json_converter<int8_t> {
    static std::string to_json_string(int8_t arg)
    {
        std::ostringstream ss;
        ss << int(arg);
        return ss.str();
    }
    static bool from_json(const Json::Value& jsonValue, int8_t& arg) {
        return from_json_helper<int8_t>::from_json(jsonValue, arg);
    }
};

// unsigned char should be converted to number
template <>
struct json_converter<uint8_t> {
    static std::string to_json_string(uint8_t arg)
    {
        std::ostringstream ss;
        ss << uint32_t(arg);
        return ss.str();
    }
    static bool from_json(const Json::Value& jsonValue, uint8_t& arg) {
        return from_json_helper<uint8_t>::from_json(jsonValue, arg);
    }
};

// char should be converted to string
template <>
struct json_converter<char> {
    static std::string to_json_string(char arg)
    {
        std::ostringstream ss;
        ss << "\"" << arg << "\"";
        return ss.str();
    }
    static bool from_json(const Json::Value& jsonValue, char& arg) {
        return from_json_helper<char>::from_json(jsonValue, arg);
    }
};

template <typename T>
struct json_converter<T, metafunction::e_type_group::enumerator> {
    using underlying_type = typename std::underlying_type<T>::type;

    static std::string to_json_string(T arg)
    {
        std::ostringstream ss;
        ss << static_cast<underlying_type>(arg);
        return ss.str();
    }
    static bool from_json(const Json::Value& jsonValue, T& arg)
    {
        underlying_type underlying_value;
        if (json_converter<underlying_type>::from_json(jsonValue, underlying_value)) {
            arg = T(underlying_value);
            return true;
        }
        return false;
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

    static bool from_json(const Json::Value& jsonValue, T& obj)
    {
        if (!jsonValue.isObject()) {
            return false;
        }
        T tempObj(obj);
        if (json_converter_helper<T>::members_from_json(jsonValue, tempObj)) {
            obj = tempObj;
            return true;
        }
        return false;
    }
};
/** ------------------------------------------------------------------------------ */
} //namespace detail

} //namespace metafunction
