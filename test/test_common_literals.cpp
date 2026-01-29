#include <common_literals.h>
#include <stdlib.h>

#include <type_traits>

auto main() -> int {
	if (32_ch != ' ') return EXIT_FAILURE;
	if constexpr (!std::is_same_v<decltype(32_u8), std::uint8_t>) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}