#include <common.h>

#include <iostream>
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

#if __cplusplus >= 202002L
	{
		std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
		auto [ymd1, hms1] = common::get_year_month_day_hh_mm_ss(t);
		t += std::chrono::seconds(1) + std::chrono::days(1);
		auto [ymd2, hms2] = common::get_year_month_day_hh_mm_ss(t);

		if (ymd2.day() - ymd1.day() != std::chrono::days(1)) return EXIT_FAILURE;
		if (hms2.seconds() - hms1.seconds() != std::chrono::seconds(1)) return EXIT_FAILURE;
	}
#endif

	return EXIT_SUCCESS;
}