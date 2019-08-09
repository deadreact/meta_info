//
// Created by Dmytro Gyrba on 10/29/18.
// Copyright (c) 2018 PacificInteractive. All rights reserved.
//

#pragma once
#include "detail/metaconfig.hpp"

template <typename T, typename D>
struct decorator
{
    using decorated_type = T;
    using decorator_type = decorator<T, D>;
    T decorated_value;

    inline decorator()
        : decorated_value(D::initial())
    {}

    inline decorator(const decorated_type& value)
        : decorated_value(D::set(value))
    {}
    inline decorator_type& operator=(const decorated_type& value)
    {
        decorated_value = D::set(value);
        return *this;
    }
    inline decorator(decorated_type&& value) NOEXCEPT
        : decorated_value(D::set(std::move(value)))
    {}

    inline decorator& operator=(decorated_type&& value) NOEXCEPT
    {
        decorated_value = D::set(std::move(value));
        return *this;
    }

    inline void reset() {
        decorated_value = D::initial();
    }

    inline operator T() const { return D::get(decorated_value); }

    inline bool operator==(const decorator_type& other) const { return decorated_value == other.decorated_value; }
    inline bool operator!=(const decorator_type& other) const { return !operator==(other); }
};

