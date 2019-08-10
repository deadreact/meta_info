#pragma once

#include "metaconfig.hpp"
#include <vector>
#include <iostream>
#include <array>

//temporary here
namespace ns42
{
CONSTEXPR inline int match(const char* str, char target) {
    return (*str == target) ? 0 : 1 + match(str + 1, target);
}

CONSTEXPR inline int match0(const char* str, char target, int n = 0) {
    return (n >= 0 && str[n] && target) ? ((str[n] == target) ? n : match0(str, target, n + 1)) : -1;
}

CONSTEXPR inline int matchseq(const char* str, const char* target) {
    return (match0(target, *str) >= 0) ? 0 : 1 + matchseq(str + 1, target);
}

CONSTEXPR inline int matchseq0(const char* str, const char* target, int n = 0) {
    return (str && target && str[n] && *target) ? ((match0(target, str[n]) >= 0) ? n : matchseq0(str, target, n + 1)) : -1;
}

CONSTEXPR inline int rmatch(const char* str, char target) {
    return (*str != target) ? 0 : 1 + rmatch(str + 1, target);
}

CONSTEXPR inline int rmatch0(const char* str, char target, int n = 0) {
    return (n >= 0 && str[n] && target) ? ((str[n] != target) ? n : rmatch0(str, target, n + 1)) : -1;
}

CONSTEXPR inline int rmatchseq(const char* str, const char* target) {
    return (match0(target, *str) < 0) ? 0 : 1 + rmatchseq(str + 1, target);
}

CONSTEXPR inline int rmatchseq0(const char* str, const char* target, int n = 0) {
    return (str && target && str[n] && *target) ? ((match0(target, str[n]) < 0) ? n : rmatchseq0(str, target, n + 1)) : -1;
}

CONSTEXPR inline int strlen(const char* str) {
    return match(str, '\0');
}

struct string_view
{
    CONSTEXPR inline string_view() NOEXCEPT : m_begin(nullptr), m_end(m_begin) {}
    CONSTEXPR inline string_view(const char* begin, const char* end) NOEXCEPT : m_begin(begin), m_end(end) {}
    CONSTEXPR inline string_view(const char* begin, size_t size) NOEXCEPT : m_begin(begin), m_end(begin + size) {}
    CONSTEXPR inline string_view(const char* cstring) NOEXCEPT : m_begin(cstring), m_end(cstring + strlen(cstring)) {}
    template <size_t N>
    CONSTEXPR inline string_view(const char(&arr)[N]) NOEXCEPT : m_begin(arr), m_end(arr + N) {}

    CONSTEXPR inline size_t size() const NOEXCEPT { return reversed() ? m_begin - m_end : m_end - m_begin; }
    CONSTEXPR inline const char* begin() const NOEXCEPT { return m_begin; }
    CONSTEXPR inline const char* end() const NOEXCEPT { return m_end; }
    CONSTEXPR inline const char* cbegin() const NOEXCEPT { return begin(); }
    CONSTEXPR inline const char* cend() const NOEXCEPT { return end(); }
    CONSTEXPR inline const char* rbegin() const NOEXCEPT { return m_end - 1; }
    CONSTEXPR inline const char* rend() const NOEXCEPT { return m_begin - 1; }
    CONSTEXPR inline const char* crbegin() const NOEXCEPT { return rbegin(); }
    CONSTEXPR inline const char* crend() const NOEXCEPT { return rend(); }
    CONSTEXPR inline bool empty() const NOEXCEPT { return m_begin == m_end; }
    CONSTEXPR inline bool reversed() const NOEXCEPT { return m_begin > m_end; }

    CONSTEXPR inline string_view get_reversed() const NOEXCEPT { return string_view(crbegin(), crend()); }
    inline string_view& reverse() NOEXCEPT { return *this = get_reversed(); }

    inline bool operator==(const string_view& other) const NOEXCEPT {
        if (size() != other.size()) {
            return false;
        }
        const char *it1 = m_begin;
        for (const char *it2 = other.m_begin; *it1 == *it2 && it1 != m_end; ++it1, ++it2);
        return it1 == m_end;
    }
    inline bool operator!=(const string_view& other) const NOEXCEPT {
        return !operator==(other);
    }
    inline bool operator<(const string_view& other) const NOEXCEPT {
        return m_begin < other.m_begin || (m_begin == other.m_begin && size() < other.size());
    }

    CONSTEXPR inline char operator[](size_t pos) const NOEXCEPT { return m_begin[pos]; }
    CONSTEXPR inline char at(size_t pos) const
    {
        return pos >= size()
            ? (throw std::out_of_range("string_view::at"), m_begin[0])
            : m_begin[pos];
    }
    CONSTEXPR inline char front() const NOEXCEPT { return *cbegin(); }
    CONSTEXPR inline char back() const NOEXCEPT { return *crend(); }

    inline std::string tostring() const {
        return std::string(m_begin, size());
    }
private:
    const char* m_begin;
    const char* m_end;
};
} // namespace ns42

inline std::string operator+(const std::string& str, const ns42::string_view& str_view) {
    return str + str_view.tostring();
}

inline std::string& operator+=(std::string& str, const ns42::string_view& str_view) {
    return str += str_view.tostring();
}

inline std::ostream& operator<<(std::ostream& os, const ns42::string_view& str_view) {
    os << str_view.tostring();
    return os;
}



namespace metafunction {
namespace detail {
/** ------------------------------------------------------------------------------------------- */
CONSTEXPR static const char* mem_name_chars_all = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
CONSTEXPR static const char* mem_name_chars_nom = "_abcdefghijklnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
CONSTEXPR static const char* mem_name_symbols   = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
std::vector<std::string> wrap_members(const char* str);

template <size_t N>
std::array<ns42::string_view, N> wrap_to_array(const char* str)
{
    std::array<ns42::string_view, N> arr;

    for (size_t i = 0; i < N; i++)
    {
#if defined(CUT_m_FROM_NAMES) && CUT_m_FROM_NAMES
        int begin = ns42::matchseq0(str, mem_name_chars_nom);
        
//        qt_assert(begin >= 0);
        if (begin > 0)
        {
            if (str[begin-1] == 'm')
            {
                if (str[begin] == '_') {
                    begin++;
                } else {
                    begin--;
                }
            }
        }
#else
        int begin = ns42::matchseq0(str, mem_name_chars_all);
#endif //CUT_m_FROM_NAMES
        str += begin;
        int end = ns42::rmatchseq0(str, mem_name_symbols);
        if (end > 0) {
            arr[i] = ns42::string_view(str, str + end);
            end = ns42::match0(str, ',');
            str += end + 1;
        } else {
            arr[i] = ns42::string_view(str);
        }
    }
    return arr;
}
/** ------------------------------------------------------------------------------------------- */
} //namespace detail
} //namespace metafunction
