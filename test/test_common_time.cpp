#include <chrono>
#include <iostream>

#include "common_time.h"

auto main() -> int {
	std::chrono::time_point<std::chrono::system_clock> const now = std::chrono::system_clock::now();
	std::chrono::system_clock::duration const duration_system_clock = now.time_since_epoch();
	std::chrono::duration<std::uint64_t, std::nano> const duration_uint64_t = std::chrono::duration_cast<std::chrono::duration<std::uint64_t, std::nano>>(duration_system_clock);
	std::uint64_t const value = duration_uint64_t.count();
	std::chrono::duration<std::uint64_t, std::nano> const duration_uint64_t2 = std::chrono::duration<std::uint64_t, std::nano>{value};
	std::chrono::system_clock::duration const duration_system_clock2 = std::chrono::duration_cast<std::chrono::system_clock::duration>(duration_uint64_t2);
	std::chrono::time_point<std::chrono::system_clock> const now2 = std::chrono::time_point<std::chrono::system_clock>{duration_system_clock2};

	std::cout << now << std::endl;
	std::cout << now2 << std::endl;
	std::cout << duration_system_clock << std::endl;
	std::cout << duration_system_clock2 << std::endl;
	std::cout << duration_uint64_t << std::endl;
	std::cout << duration_uint64_t2 << std::endl;
	std::cout << value << std::endl;

	std::uint64_t const ns = common::to_uint64_t(now);
	auto const restored = common::from_uint64_t(ns);

	if (auto const diff = restored.time_since_epoch() - now.time_since_epoch(); diff != std::chrono::system_clock::duration::zero()) return EXIT_FAILURE;

	std::cout << common::to_string(now) << std::endl;
	std::cout << common::to_string(now2) << std::endl;

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