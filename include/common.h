#pragma once

#include <algorithm>
#include <array>
#include <queue>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace common {
	// static_cast any signed type to their unsigned equivalent
	[[maybe_unused]] inline constexpr auto as_unsigned(std::integral auto a) { return static_cast<typename std::make_unsigned<decltype(a)>::type>(a); }

	// static_cast any unsigned type to their signed equivalent
	[[maybe_unused]] inline constexpr auto as_signed(std::integral auto a) { return static_cast<typename std::make_signed<decltype(a)>::type>(a); }

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

	// Hash std::arrays for std::unordered_map -> std::unordered_map<std::array<..., ...>, ..., ArrayHasher>
	struct [[maybe_unused]] ArrayHasher {
		template <typename TYPE, std::size_t SIZE>
		std::size_t operator()(std::array<TYPE, SIZE> const &arr) const {
			std::size_t h = 0;
			for (TYPE const e : arr) {
				h ^= std::hash<TYPE>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
			}
			return h;
		}
	};

	// use this to store key-value pairs of any kind and to access only the highest key.
	template <typename Key, typename Value>
	class [[maybe_unused]] pair_priority_queue
	    : public std::priority_queue<std::pair<Key, Value>, std::vector<std::pair<Key, Value>>, decltype([](std::pair<Key, Value> const &a, std::pair<Key, Value> const &b) { return a.first < b.first; })> {};
}  // namespace common