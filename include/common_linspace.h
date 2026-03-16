#pragma once
#include <array>
#include <cstdint>
#include <span>

#if defined(__cpp_lib_ranges_enumerate)
#include <ranges>
#endif

namespace common {
	template <std::floating_point T, std::size_t N>
	std::array<T, N> linspace(T from, T to) {
		std::array<T, N> result;

#if defined(__cpp_lib_ranges_enumerate)
		for (T const step = (to - from) / (N - 1); auto const& [i, e] : std::ranges::views::enumerate(result)) {
#else
		auto i = -1;
		for (T const step = (to - from) / (N - 1); auto& e : result) {
			++i;
#endif
			e = from + i * step;
		}

		return result;
	}

	template <std::floating_point T>
	void linspace(T const from, T const to, std::size_t const N, std::span<T> result) {
#if defined(__cpp_lib_ranges_enumerate)
		for (double const step = (to - from) / (N - 1); auto const& [i, e] : std::ranges::views::enumerate(result)) {
#else
		auto i = -1;
		for (double const step = (to - from) / (N - 1); auto& e : result) {
			++i;
#endif
			e = from + i * step;
		}
	}

	template <std::floating_point T, std::size_t N>
	std::array<T, N> linspace_exclusive(T from, T to) {
		std::array<T, N> result;

#if defined(__cpp_lib_ranges_enumerate)
		for (T const step = (to - from) / N; auto const& [i, e] : std::ranges::views::enumerate(result)) {
#else
		auto i = -1;
		for (T const step = (to - from) / N; auto& e : result) {
			++i;
#endif
			e = from + i * step;
		}

		return result;
	}
}  // namespace common