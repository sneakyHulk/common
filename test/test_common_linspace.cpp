#include <common_linspace.h>

auto main() -> int {
	if (auto const a = common::linspace<5>(0.0, 4.0); !(a[0] == 0.0 && a[1] == 1.0 && a[2] == 2.0 && a[3] == 3.0 && a[4] == 4.0)) return EXIT_FAILURE;
	if (auto const a = common::linspace_exclusive<4>(0.0, 4.0); !(a[0] == 0.0 && a[1] == 1.0 && a[2] == 2.0 && a[3] == 3.0)) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}