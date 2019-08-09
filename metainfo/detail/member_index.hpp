//
// Created by Dmytro Gyrba on 2019-02-04.
// Copyright (c) 2019 PacificInteractive. All rights reserved.
//

#pragma once

#include "metaconfig.hpp"

namespace metafunction {
namespace detail {

// types mismatch T != Arg
template <int N, typename T, typename Arg, typename... Args>
struct member_index_helper {
    CONSTEXPR static const int get_value(const T& target, const Arg& arg, const Args&... args) {
        return member_index_helper<N+1, T, Args...>::get_value(target, args...);
    }
};

// types match T == T
template <int N, typename T, typename... Args>
struct member_index_helper<N, T, T, Args...> {
    CONSTEXPR static const int get_value(const T& target, const T& arg, const Args&... args) {
        return &target == &arg ? N : member_index_helper<N+1, T, Args...>::get_value(target, args...);
    }
};

// types mismatch T != Arg
template <int N, typename T, typename Arg>
struct member_index_helper<N, T, Arg> {
    CONSTEXPR static const int get_value(const T& target, const Arg& arg) {
        return -1;
    }
};

// types match T == T
template <int N, typename T>
struct member_index_helper<N, T, T> {
    CONSTEXPR static const int get_value(const T& target, const T& arg) {
        return &target == &arg ? N : -1;
    }
};

template <typename T, typename... Args>
CONSTEXPR int get_member_index(const T& target, const Args&... args) {
    return member_index_helper<0, T, Args...>::get_value(target, args...);
}

}//namespace detail
}//namespace metafunction
