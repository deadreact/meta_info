#pragma once

#include <set>
#include "member_count.hpp"

namespace metafunction {
/** --------------------------------------------------------------------------------------------------------------- */
template <typename T> using difference_type = std::set<T>;
/** --------------------------------------------------------------------------------------------------------------- */
namespace detail {
/** --------------------------------------------------------------------------------------------------------------- */
template <typename T, int N = subelements_count_obj<T>::value - 1>
struct compare_helper {
    inline static bool cmp1(const T& this_, const T& that_) {
        return std::get<N>(this_) == std::get<N>(that_) && compare_helper<T, N-1>::cmp1(this_, that_);
    }
    inline static void cmp2(difference_type<typename T::e>& lst, const T& this_, const T& that_) {
        if (std::get<N>(this_) != std::get<N>(that_)) {
            lst.insert(typename T::e(N));
        }
        compare_helper<T, N-1>::cmp2(lst, this_, that_);
    }
};
template <typename T>
struct compare_helper<T, 0> {
    inline static bool cmp1(const T& this_, const T& that_) {
        return std::get<0>(this_) == std::get<0>(that_);
    }
    inline static void cmp2(difference_type<typename T::e>& lst, const T& this_, const T& that_) {
        if (std::get<0>(this_) != std::get<0>(that_)) {
            lst.insert(typename T::e(0));
        }
    }
};
/** --------------------------------------------------------------------------------------------------------------- */
/************************************************************************/
} //namespace detail
} //namespace metafunction
