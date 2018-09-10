#pragma once

#include "detail/more_traits.hpp"
#include <type_traits>
namespace metafunction
{
    enum class e_type_group {
        invalid = 0,
        list,
        structure,
        single,
        pair,
        enumerator
    };

    template <typename T>
    struct type_group_detect {
        static const e_type_group type_group =
                std::is_arithmetic<T>::value || std::is_same<T, std::string>::value
                    ? e_type_group::single
                    : (is_pair<T>::value
                            ? e_type_group::pair
                            : (is_container<T>::value
                                    ? e_type_group::list
                                    : (is_structure<T>::value
                                            ? e_type_group::structure
                                            : (std::is_enum<T>::value
                                                    ? e_type_group::enumerator
                                                    : e_type_group::invalid))));
    };

} //namespace metafunction

