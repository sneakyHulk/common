#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>

#include "common.h"
#include "common_output.h"

namespace common {
	template <typename Clock = std::chrono::system_clock>
	std::uint64_t to_uint64_t(std::chrono::time_point<Clock> const& tp) {
		typename Clock::duration const duration_system_clock = tp.time_since_epoch();
		std::chrono::duration<std::uint64_t, std::nano> const duration_uint64_t = std::chrono::duration_cast<std::chrono::duration<std::uint64_t, std::nano>>(duration_system_clock);
		std::uint64_t const value = duration_uint64_t.count();

		return value;
	}

	template <typename Clock = std::chrono::system_clock>
	std::chrono::time_point<Clock> from_uint64_t(std::uint64_t const ns) {
		std::chrono::duration<std::uint64_t, std::nano> const duration_uint64_t = std::chrono::duration<std::uint64_t, std::nano>{ns};
		typename Clock::duration const duration_system_clock = std::chrono::duration_cast<typename Clock::duration>(duration_uint64_t);
		std::chrono::time_point<Clock> const now = std::chrono::time_point<Clock>{duration_system_clock};

		return now;
	}

	template <typename Clock = std::chrono::system_clock>
	std::string to_string(std::chrono::time_point<Clock> const& tp) {
		auto [ymd, hms] = common::get_year_month_day_hh_mm_ss(tp);

		return common::stringprint(static_cast<int>(ymd.year()), "-", std::setw(2), std::setfill('0'), static_cast<unsigned>(ymd.month()), "-", std::setw(2), std::setfill('0'), static_cast<unsigned>(ymd.day()), "_", std::setw(2),
		    std::setfill('0'), hms.hours().count(), "-", std::setw(2), std::setfill('0'), hms.minutes().count(), "-", std::setw(2), std::setfill('0'), hms.seconds().count(), "_", std::setw(19), std::setfill('0'), to_uint64_t(tp));
	}
}  // namespace common