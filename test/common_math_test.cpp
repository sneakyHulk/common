#include <common.h>
#include <common_math.h>

auto main() -> int {
	if (common::is_prime(common::as_unsigned(11)) != true && common::is_prime(11U) != true) return 1;

	// euclidean modulo:
	if (common::euclidean_mod(-2, -3) != 1) return 1;
	if (common::euclidean_mod(-2, 3) != 1) return 1;
	if (common::euclidean_mod(2, -3) != 2) return 1;
	if (common::euclidean_mod(2, 3) != 2) return 1;

	// floored modulo:
	if (common::floored_mod(-2, -3) != -2) return 1;
	if (common::floored_mod(-2, 3) != 1) return 1;
	if (common::floored_mod(2, -3) != -1) return 1;
	if (common::floored_mod(2, 3) != 2) return 1;

	// truncated modulo:
	if (-2 % -3 != -2) return 1;
	if (-2 % 3 != -2) return 1;
	if (2 % -3 != 2) return 1;
	if (2 % 3 != 2) return 1;

	if (common::integer_power(2, 9U) != 512) return 1;
	if (common::integer_power(2, common::as_unsigned(9)) != 512) return 1;
	if (common::integer_power(-2, 9U) != -512) return 1;
	if (common::integer_power(-2, common::as_unsigned(9)) != -512) return 1;

	if (common::get_digit_10(-123456, 2U) != 4) return 1;
	if (common::get_digit_10(123456, 0U) != 6) return 1;

	if (common::integer_sqrt32(25) != 5) return 1;
	if (common::integer_sqrt32(24) != 4) return 1;
}