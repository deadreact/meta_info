#pragma once

#include "../type_group.hpp"

namespace metafunction {
namespace detail {
/** ------------------------------------------------------------------------------- */
template <typename T, e_type_group = type_group_detect<T>::type_group>
struct subelements_count_obj {
    CONSTEXPR static const size_t value = 0;
    CONSTEXPR static size_t get_value(const T* = nullptr) { return value; }
};
template <typename T>
struct subelements_count_obj<T, e_type_group::pair> {
    CONSTEXPR static const size_t value = 2;
    CONSTEXPR static size_t get_value(const T* = nullptr) { return value; }
};
template <typename T>
struct subelements_count_obj<T, e_type_group::structure> {
    CONSTEXPR static const size_t value = std::tuple_size<typename T::types_as_tuple>::value;
    CONSTEXPR static size_t get_value(const T* = nullptr) { return value; }
};
template <typename T>
struct subelements_count_obj<T, e_type_group::container> {
    CONSTEXPR static const size_t value = 0;
    static size_t get_value(const T* obj = nullptr) { return obj ? obj->size(): 0; }
};

/** ------------------------------------------------------------------------------- */
} //namespace detail
} //namespace metafunction



namespace std
{
	// std::get<N>(<struct declared with metainfo>)
    template <int N, typename T>
    inline const typename std::enable_if<is_structure_group<typename std::remove_reference<T>::type>::value, typename T::template member_type<N>>::type&
    get(const T& obj) { return obj.template get_member<N>(); }

    template <int N, typename T>
    inline typename std::enable_if<is_structure_group<typename std::remove_reference<T>::type>::value, typename T::template member_type<N>>::type&
    get(T& obj) { return obj.template get_member<N>(); }

    template <typename T, typename Seq, Seq N, Seq... Ns>
    struct deep_index_helper {
		CONSTEXPR static const int n = int(N);
		using next_type = typename std::remove_reference<typename std::remove_cv<decltype(std::get<n>(std::declval<T&>()))>::type>::type;
		using type = typename deep_index_helper<next_type, Seq, Ns...>::type;

		const type& get(const T& obj) const { return deep_index_helper<next_type, Seq, Ns...>().get(std::get<n>(obj)); }
		      type& get(T& obj)             { return deep_index_helper<next_type, Seq, Ns...>().get(std::get<n>(obj)); }
    };

    template <typename T, typename Seq, Seq N>
    struct deep_index_helper<T, Seq, N> {
		CONSTEXPR static const int n = int(N);
		using type = typename std::remove_reference<typename std::remove_cv<decltype(std::get<n>(std::declval<T&>()))>::type>::type;
        
		const type& get(const T& obj) const { return std::get<n>(obj); }
		      type& get(T& obj)             { return std::get<n>(obj); }
    };

	// std::get<N...>(<anything>)
	// example:
	// auto data = std::make_tuple(std::make_tuple(42, 'a'), std::array<int, 2>{11, 212});
	// std::get<0, 1>(data) // 'a'
	// std::get<1, 1>(data) // 212
    template <int... Ns, typename T>
    auto get(T&& obj) -> decltype(deep_index_helper<T, int, Ns...>().get(std::forward<T>(obj))) {
        return deep_index_helper<T, int, Ns...>().get(std::forward<T>(obj));
    }
} //namespace std
