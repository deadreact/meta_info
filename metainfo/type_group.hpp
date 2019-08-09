#pragma once

#include "detail/more_traits.hpp"
#include <type_traits>
namespace metafunction
{
    enum class e_type_group {
        invalid = 0,
        container,
        structure,
        single,
        pair,
        enumerator
    };

    template <typename T, bool is_decorator = has_decorated_type<T>::value>
    struct type_group_detect {
        static const e_type_group type_group =
                std::is_arithmetic<T>::value || std::is_same<T, std::string>::value
                    ? e_type_group::single
                    : (is_pair<T>::value
                            ? e_type_group::pair
                            : (is_container<T>::value
                                    ? e_type_group::container
                                    : (is_structure<T>::value
                                            ? e_type_group::structure
                                            : (std::is_enum<T>::value
                                                    ? e_type_group::enumerator
                                                    : e_type_group::invalid))));
    };

    template <typename T>
    struct type_group_detect<T, true> {
        static const e_type_group type_group = type_group_detect<typename T::decorated_type>::type_group;
    };

} //namespace metafunction

template <typename T>
struct is_structure_group : std::integral_constant<bool, metafunction::type_group_detect<T>::type_group == metafunction::e_type_group::structure> {};

