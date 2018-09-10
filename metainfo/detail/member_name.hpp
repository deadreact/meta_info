#pragma once

#include <vector>
#include <string>

#define CUT_m_FROM_NAMES 0

namespace metafunction {
namespace detail {
/** ------------------------------------------------------------------------------------------- */
template <int N>
bool sep_names(const char (&string)[N], char (&arr)[N]) {
    for (const char* it = string; it != (string + N); it++) {
        if (*it == ',') {
            *arr = 0;
        } else if (*it != ' ') {
            *arr = *it;
        }
        arr++;
    }
    return false;
}

std::vector<std::string> wrap_members(const char* str);
/** ------------------------------------------------------------------------------------------- */
} //namespace detail
} //namespace metafunction
