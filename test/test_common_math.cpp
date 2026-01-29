#include <common.h>
#include <common_math.h>
#include <common_output.h>

auto main() -> int {
	if (common::is_prime(common::as_unsigned(11)) != true && common::is_prime(11U) != true) return EXIT_FAILURE;

	// euclidean modulo:
	if (common::euclidean_mod(-2, -3) != 1) return EXIT_FAILURE;
	if (common::euclidean_mod(-2, 3) != 1) return EXIT_FAILURE;
	if (common::euclidean_mod(2, -3) != 2) return EXIT_FAILURE;
	if (common::euclidean_mod(2, 3) != 2) return EXIT_FAILURE;

	// floored modulo:
	if (common::floored_mod(-2, -3) != -2) return EXIT_FAILURE;
	if (common::floored_mod(-2, 3) != 1) return EXIT_FAILURE;
	if (common::floored_mod(2, -3) != -1) return EXIT_FAILURE;
	if (common::floored_mod(2, 3) != 2) return EXIT_FAILURE;

	// truncated modulo:
	if (-2 % -3 != -2) return EXIT_FAILURE;
	if (-2 % 3 != -2) return EXIT_FAILURE;
	if (2 % -3 != 2) return EXIT_FAILURE;
	if (2 % 3 != 2) return EXIT_FAILURE;

	if (common::ipow(2, 9U) != 512) return EXIT_FAILURE;
	if (common::ipow(2, common::as_unsigned(9)) != 512) return EXIT_FAILURE;
	if (common::ipow(-2, 9U) != -512) return EXIT_FAILURE;
	if (common::ipow(-2, common::as_unsigned(9)) != -512) return EXIT_FAILURE;

	if (common::ipow<9>(2) != 512) return EXIT_FAILURE;
	if (common::ipow<9>(2) != 512) return EXIT_FAILURE;
	if (common::ipow<9>(-2) != -512) return EXIT_FAILURE;
	if (common::ipow<9>(-2) != -512) return EXIT_FAILURE;

	if (common::get_digit_10(-123456, 2U) != 4) return EXIT_FAILURE;
	if (common::get_digit_10(123456, 0U) != 6) return EXIT_FAILURE;

	if (common::integer_sqrt32(25) != 5) return EXIT_FAILURE;
	if (common::integer_sqrt32(24) != 4) return EXIT_FAILURE;

	if (common::nCr(7U, 4U) != 35) return EXIT_FAILURE;
	if (common::nCr(common::as_unsigned(7), common::as_unsigned(3)) != 35) return EXIT_FAILURE;

	if (common::number_of_digits_10<int>(0) != 1) return EXIT_FAILURE;
	if (common::number_of_digits_10<int>(89090) != 5) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}
