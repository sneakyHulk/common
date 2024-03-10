#include <common.h>
#include <common_math.h>

#include <iostream>

auto main() -> int {
	if (is_prime(common::as_unsigned(11)) != true && is_prime(11U) != true) return 1;

	// euclidean modulo:
	if (euclidean_mod(-2, -3) != 1) return 1;
	if (euclidean_mod(-2, 3) != 1) return 1;
	if (euclidean_mod(2, -3) != 2) return 1;
	if (euclidean_mod(2, 3) != 2) return 1;

	// floored modulo:
	if (floored_mod(-2, -3) != -2) return 1;
	if (floored_mod(-2, 3) != 1) return 1;
	if (floored_mod(2, -3) != -1) return 1;
	if (floored_mod(2, 3) != 2) return 1;

	// truncated modulo:
	if (-2 % -3 != -2) return 1;
	if (-2 % 3 != -2) return 1;
	if (2 % -3 != 2) return 1;
	if (2 % 3 != 2) return 1;

	if (int_power(2, 9U) != 512) return 1;
	if (int_power(2, common::as_unsigned(9)) != 512) return 1;
	if (int_power(-2, 9U) != -512) return 1;
	if (int_power(-2, common::as_unsigned(9)) != -512) return 1;

	if (get_digit_10(-123456, 2U) != 4) return 1;
	if (get_digit_10(123456, 0U) != 6) return 1;
}