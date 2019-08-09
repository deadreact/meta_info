//
// Created by Dmytro Gyrba on 2019-04-10.
// Copyright (c) 2019 PacificInteractive. All rights reserved.
//

#pragma once

#include "member_name.hpp"
#include "member_offset.hpp"

namespace metafunction {
namespace detail {

template<typename T>
struct metadata {
    using names_type = std::array<ns42::string_view, T::member_count>;
    using offsets_type = std::array<offset_type, T::member_count>;

    names_type names;
    offsets_type offsets;

    metadata(names_type&& names, offsets_type&& offsets)
        : names(std::move(names))
        , offsets(std::move(offsets))
    {}

    // TODO: make complexity O(1), ideally in compile-time
    inline int member_index(offset_type offset) const
    {
        auto it = std::find(offsets.begin(), offsets.end(), offset);
        return (it != offsets.end()) ? it - offsets.begin() : -1;
    }

    // TODO: make complexity O(1), ideally in compile-time
    inline int member_index(const char *name) const
    {
        ns42::string_view name_view(name);
        auto it = std::find(names.begin(), names.end(), name_view);
        return (it != names.end()) ? it - names.begin() : -1;
    }
};

} //namespace detail
} //namespace metafunction
