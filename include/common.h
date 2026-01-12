#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <queue>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

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
#if __cplusplus >= 202002L
	template <typename Key, typename Value>
	class [[maybe_unused]] pair_priority_queue
	    : public std::priority_queue<std::pair<Key, Value>, std::vector<std::pair<Key, Value>>, decltype([](std::pair<Key, Value> const &a, std::pair<Key, Value> const &b) { return a.first < b.first; })> {};
#endif

	inline std::tuple<std::chrono::year_month_day, std::chrono::hh_mm_ss<decltype(std::chrono::seconds())>> get_year_month_day_hh_mm_ss(std::chrono::system_clock::time_point const &t = std::chrono::system_clock::now()) {
#if __cpp_lib_chrono >= 201907L
		auto const created = std::chrono::zoned_time{std::chrono::current_zone(), t}.get_local_time();
		auto const day = std::chrono::floor<std::chrono::days>(created);
		auto const second = std::chrono::floor<std::chrono::seconds>(created - day);
		std::chrono::hh_mm_ss const hms{second};
		std::chrono::year_month_day const ymd{day};

		return {ymd, hms};
#else
		std::time_t const tt = std::chrono::system_clock::to_time_t(t);
		std::tm local_tm{};
		localtime_r(&tt, &local_tm);

		std::chrono::year_month_day ymd{std::chrono::year{local_tm.tm_year + 1900}, std::chrono::month{static_cast<unsigned>(local_tm.tm_mon + 1)}, std::chrono::day{static_cast<unsigned>(local_tm.tm_mday)}};

		std::chrono::hh_mm_ss<std::chrono::seconds> hms{std::chrono::seconds{local_tm.tm_hour * 3600 + local_tm.tm_min * 60 + local_tm.tm_sec}};

		return {ymd, hms};
#endif
	}
}  // namespace common