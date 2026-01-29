#pragma once

#include <common_output.h>

#include <array>
#include <cstdint>
#include <span>

namespace common {
	template <typename T, std::size_t N>
	class ring_buffer : protected std::array<T, N> {
	   public:
		template <std::size_t max_sub_array_size = N>
		constexpr std::span<std::uint8_t> linear_sub_array() requires(max_sub_array_size <= N) {
			return {std::array<T, N>::begin() + index, std::array<T, N>::begin() + std::min(index + max_sub_array_size, N)};
		}
		constexpr std::span<std::uint8_t> linear_sub_array() { return {std::array<T, N>::begin() + index, std::array<T, N>::end()}; }
		constexpr void rotate(std::size_t const n) {
			index = (index + n) % N;
			if (n > pops)
				pops = 0;
			else
				pops -= n;
		}
		constexpr void push_back(T const value) {
			std::array<T, N>::operator[](index) = value;
			index = (index + 1) % N;
			if (pops) --pops;
		}

		[[nodiscard]] constexpr std::size_t size() const { return N - pops; }

		constexpr std::array<T, N> const& array() { return *this; }

		constexpr T const& back() const {
			if (size() == 0) common::println_error_loc("size() == 0");
			return operator[](size() - 1);
		}

		constexpr T& back() {
			if (size() == 0) common::println_error_loc("size() == 0");
			return operator[](size() - 1);
		}

		constexpr T const& front() const {
			if (size() == 0) common::println_error_loc("size() == 0");
			return operator[](0);
		}

		constexpr T& front() {
			if (size() == 0) common::println_error_loc("size() == 0");
			return operator[](0);
		}

		constexpr T const& operator[](std::size_t const n) const {
			if (n >= size()) common::println_error_loc("n < size()");
			return std::array<T, N>::operator[]((index + pops + n) % N);
		}

		constexpr T& operator[](std::size_t const n) {
			if (n >= size()) common::println_error_loc("n < size()");
			return std::array<T, N>::operator[]((index + pops + n) % N);
		}

		constexpr void pop() {
			pops = std::min(pops + 1, N);
			if (pops + 1 > N) common::println_error_loc("pops > N");
		}
		constexpr void pop(std::size_t const n) {
			if (pops + n > N) common::println_error_loc("pops > N");
			pops = std::min(pops + n, N);
		}

	   private:
		std::size_t index = 0;
		std::size_t pops = N;
	};
}  // namespace common