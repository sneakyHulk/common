#pragma once
#include <cmath>
#include <concepts>

[[maybe_unused]] constexpr bool is_prime(std::unsigned_integral auto const n) {
	if (n == 2U || n == 3U) return true;

	if (n <= 1U || n % 2U == 0U || n % 3U == 0U) return false;

	for (auto i = 5U; i * i <= n; i += 6U) {
		if (n % i == 0U || n % (i + 2U) == 0U) return false;
	}

	return true;
}

[[maybe_unused]] constexpr auto int_power(std::integral auto const a, std::unsigned_integral auto b) {
	typename std::remove_const<decltype(a)>::type res = 1;
	while (b--) res *= a;
	return res;
}

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

[[maybe_unused]] constexpr auto floored_mod(std::signed_integral auto a, std::signed_integral auto b) { return ((a % b) + b) % b; }
[[maybe_unused]] constexpr auto floored_mod(std::unsigned_integral auto a, std::signed_integral auto b) { return ((a % b) + b) % b; }
[[maybe_unused]] constexpr auto floored_mod(std::signed_integral auto a, std::unsigned_integral auto b) { return ((a % b) + b) % b; }
[[maybe_unused]] constexpr auto floored_mod(std::unsigned_integral auto a, std::unsigned_integral auto b) { return a % b; }

[[maybe_unused]] constexpr auto get_digit_10(std::unsigned_integral auto const value, std::unsigned_integral auto const digit) { return (value / int_power(10, digit)) % 10; }
[[maybe_unused]] constexpr auto get_digit_10(std::signed_integral auto const value, std::unsigned_integral auto const digit) { return get_digit_10(common::as_unsigned(std::abs(value)), digit); }