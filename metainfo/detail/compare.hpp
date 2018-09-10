#pragma once

#include <set>
#include "member_count.hpp"

namespace metafunction {
/** --------------------------------------------------------------------------------------------------------------- */
using difference_type = std::set<size_t>;
/** --------------------------------------------------------------------------------------------------------------- */
namespace detail {
/** --------------------------------------------------------------------------------------------------------------- */
template <typename T, int N = metafunction::detail::subelements_count_obj<T>::value - 1>
struct compare_helper {
    inline static bool cmp1(const T& this_, const T& that_) {
        return this_.template get_member<N>() == that_.template get_member<N>() && compare_helper<T, N-1>::cmp1(this_, that_);
    }
    inline static void cmp2(difference_type& lst, const T& this_, const T& that_) {
        if (this_.template get_member<N>() != that_.template get_member<N>()) {
            lst.insert(N);
        }
        compare_helper<T, N-1>::cmp2(lst, this_, that_);
    }
};
template <typename T>
struct compare_helper<T, 0> {
    inline static bool cmp1(const T& this_, const T& that_) {
        return this_.template get_member<0>() == that_.template get_member<0>();
    }
    inline static void cmp2(difference_type& lst, const T& this_, const T& that_) {
        if (this_.template get_member<0>() != that_.template get_member<0>()) {
            lst.insert(0);
        }
    }
};
/** --------------------------------------------------------------------------------------------------------------- */
/************************************************************************/
} //namespace detail
} //namespace metafunction
