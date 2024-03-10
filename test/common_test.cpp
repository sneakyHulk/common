#include <common.h>

auto main() -> int {
	if (auto arr = common::get_array_from_tuple(std::make_tuple(1, 2, 3)); arr != std::array{1, 2, 3}) return 1;
}