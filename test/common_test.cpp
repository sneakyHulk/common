#include <common.h>

#include <unordered_map>

auto main() -> int {
	if (auto arr = common::get_array_from_tuple(std::make_tuple(1, 2, 3)); arr != std::array{1, 2, 3}) return EXIT_FAILURE;

	{
		std::unordered_map<std::array<int, 2>, int, common::ArrayHasher> m;
		if (m[{1, 2}] = 3; m[{1, 2}] != 3) return EXIT_FAILURE;
	}

#if __cplusplus >= 202002L
	{
		common::pair_priority_queue<int, std::array<int, 2>> pq;
		if (pq.push(std::make_pair(1, std::array{2, 3})); pq.top().first != 1 || pq.top().second != std::array{2, 3}) return EXIT_FAILURE;
	}
#endif

	return EXIT_SUCCESS;
}