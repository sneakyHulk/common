#include <common_linspace.h>

#include <vector>

auto main() -> int {
	if (auto const a = common::linspace<double, 5>(0.0, 4.0); !(a[0] == 0.0 && a[1] == 1.0 && a[2] == 2.0 && a[3] == 3.0 && a[4] == 4.0)) return EXIT_FAILURE;
	if (auto const a = common::linspace_exclusive<double, 4>(0.0, 4.0); !(a[0] == 0.0 && a[1] == 1.0 && a[2] == 2.0 && a[3] == 3.0)) return EXIT_FAILURE;

	std::vector<double> result(5);
	common::linspace<double>(0.0, 4.0, 5, result);
	if (auto const a = result; !(a[0] == 0.0 && a[1] == 1.0 && a[2] == 2.0 && a[3] == 3.0 && a[4] == 4.0)) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}