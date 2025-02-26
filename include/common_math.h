#pragma once

#include <cinttypes>
#include <cmath>
#include <concepts>
#include <cstdint>

#include "common.h"

namespace common {
	[[maybe_unused]] constexpr bool is_prime(std::unsigned_integral auto const n) {
		if (n == 2U || n == 3U) return true;

		if (n <= 1U || n % 2U == 0U || n % 3U == 0U) return false;

		for (auto i = 5U; i * i <= n; i += 6U) {
			if (n % i == 0U || n % (i + 2U) == 0U) return false;
		}

		return true;
	}

	[[maybe_unused]] constexpr std::uint_fast8_t number_of_digits_10(std::integral auto n) {
		std::uint_fast8_t len = 0;
		do {
			++len;
		} while (n /= 10);

		return len;
	}

	[[maybe_unused]] constexpr auto integer_power(std::integral auto const a, std::unsigned_integral auto b) {
		typename std::remove_const<decltype(a)>::type res = 1;
		while (b--) res *= a;
		return res;
	}

	// modulo with only positive outcome
	[[maybe_unused]] constexpr auto euclidean_mod(std::signed_integral auto a, std::signed_integral auto b) {
		std::signed_integral auto r = a % b;
		return r >= 0 ? r : r + std::abs(b);
	}
	[[maybe_unused]] constexpr auto euclidean_mod(std::unsigned_integral auto a, std::signed_integral auto b) {
		std::signed_integral auto r = a % b;
		return r >= 0 ? r : r + std::abs(b);
	}
	[[maybe_unused]] constexpr auto euclidean_mod(std::signed_integral auto a, std::unsigned_integral auto b) {
		std::signed_integral auto r = a % b;
		return r >= 0 ? r : r + b;
	}
	[[maybe_unused]] constexpr auto euclidean_mod(std::unsigned_integral auto a, std::unsigned_integral auto b) { return a % b; }

	// see python modulo:
	[[maybe_unused]] constexpr auto floored_mod(std::signed_integral auto a, std::signed_integral auto b) { return ((a % b) + b) % b; }
	[[maybe_unused]] constexpr auto floored_mod(std::unsigned_integral auto a, std::signed_integral auto b) { return ((a % b) + b) % b; }
	[[maybe_unused]] constexpr auto floored_mod(std::signed_integral auto a, std::unsigned_integral auto b) { return ((a % b) + b) % b; }
	[[maybe_unused]] constexpr auto floored_mod(std::unsigned_integral auto a, std::unsigned_integral auto b) { return a % b; }

	[[maybe_unused]] constexpr auto get_digit_10(std::unsigned_integral auto const value, std::unsigned_integral auto const digit) { return (value / integer_power(10, digit)) % 10; }
	[[maybe_unused]] constexpr auto get_digit_10(std::signed_integral auto const value, std::unsigned_integral auto const digit) { return get_digit_10(common::as_unsigned(std::abs(value)), digit); }

	[[maybe_unused]] constexpr std::uint16_t integer_sqrt32(std::uint32_t x) {
		std::uint16_t res = 0;
		std::uint16_t add = 0x8000;
		for (std::integral auto i = 0; i < 16; ++i) {
			std::uint16_t temp = res | add;
			std::uint32_t g2 = static_cast<std::uint32_t>(temp) * static_cast<std::uint32_t>(temp);
			if (x >= g2) res = temp;
			add >>= 1;
		}
		return res;
	}

	// classic n choose r computation, will result in 1 when fed with r values greater than n.
	// when extending to signed negative values for n or r will also produce 1.
	[[maybe_unused]] constexpr auto nCr(std::unsigned_integral auto n, std::unsigned_integral auto r) {
		if (r > n - r) r = n - r;  // because C(n, r) == C(n, n - r)
		decltype(n) ans = 1;

		for (auto i = 1; i <= r; i++) {
			ans *= n - r + i;
			ans /= i;
		}

		return ans;
	}
}  // namespace common