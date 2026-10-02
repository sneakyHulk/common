#pragma once

#include <algorithm>
#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#ifdef __cpp_concepts
#include <concepts>
#endif

namespace common {
	// concept to check whether range-based for loop can be used.
#ifdef __cpp_concepts
	template <typename T>
	concept iterable = requires(T const &obj) {
		{ obj.begin(), obj.end(), ++(obj.begin()) };
	};
#endif

	// static_cast any signed type to their unsigned equivalent
#ifdef __cpp_concepts
	[[maybe_unused]] inline constexpr auto as_unsigned(std::integral auto a) { return static_cast<typename std::make_unsigned<decltype(a)>::type>(a); }
#else
	template <typename T>
	[[maybe_unused]] inline constexpr auto as_unsigned(T a) {
		return static_cast<typename std::make_unsigned<decltype(a)>::type>(a);
	}
#endif

	// static_cast any unsigned type to their signed equivalent
#ifdef __cpp_concepts
	[[maybe_unused]] inline constexpr auto as_signed(std::integral auto a) { return static_cast<typename std::make_signed<decltype(a)>::type>(a); }
#else
	template <typename T>
	[[maybe_unused]] inline constexpr auto as_signed(T a) {
		return static_cast<typename std::make_signed<decltype(a)>::type>(a);
	}
#endif

	// use a string literal "<string>" as template argument parameter
	template <std::size_t N>
	struct [[maybe_unused]] StringLiteral {
		char value[N];

		constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }
	};

	// convert a tuple to an array
	template <typename tuple_t>
	[[maybe_unused]] constexpr auto get_array_from_tuple(tuple_t &&tuple) {
		constexpr auto get_array = [](auto &&...x) { return std::array{std::forward<decltype(x)>(x)...}; };
		return std::apply(get_array, std::forward<tuple_t>(tuple));
	}

	template <std::size_t N, typename T, std::size_t... Is>
	constexpr std::array<T, N> filled_array_impl(T value, std::index_sequence<Is...>) {
		return {((void)Is, value)...};
	}

	template <std::size_t N, typename T>
	constexpr std::array<T, N> filled_array(T value) {
		return filled_array_impl<N>(value, std::make_index_sequence<N>{});
	}
}  // namespace common