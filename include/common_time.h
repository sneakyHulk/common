#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <string>
#include <tuple>

#include "common.h"
#include "common_output.h"

namespace common {
	inline std::tuple<std::chrono::year_month_day, std::chrono::hh_mm_ss<decltype(std::chrono::seconds())>> get_year_month_day_hh_mm_ss(std::chrono::system_clock::time_point const &t = std::chrono::system_clock::now()) {
#if __cpp_lib_chrono >= 201907L
		auto const created = std::chrono::zoned_time{std::chrono::current_zone(), t}.get_local_time();
		auto const day = std::chrono::floor<std::chrono::days>(created);
		auto const second = std::chrono::floor<std::chrono::seconds>(created - day);
		std::chrono::hh_mm_ss const hms{second};
		std::chrono::year_month_day const ymd{day};

		return {ymd, hms};
#else
		std::time_t const tt = std::chrono::system_clock::to_time_t(t);
		std::tm local_tm{};
		localtime_r(&tt, &local_tm);

		std::chrono::year_month_day ymd{std::chrono::year{local_tm.tm_year + 1900}, std::chrono::month{static_cast<unsigned>(local_tm.tm_mon + 1)}, std::chrono::day{static_cast<unsigned>(local_tm.tm_mday)}};

		std::chrono::hh_mm_ss<std::chrono::seconds> hms{std::chrono::seconds{local_tm.tm_hour * 3600 + local_tm.tm_min * 60 + local_tm.tm_sec}};

		return {ymd, hms};
#endif
	}

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

#ifndef ARDUINO
	template <typename Clock = std::chrono::system_clock>
	std::string to_string(std::chrono::time_point<Clock> const& tp) {
		auto [ymd, hms] = common::get_year_month_day_hh_mm_ss(tp);

		return common::stringprint(static_cast<int>(ymd.year()), "-", std::setw(2), std::setfill('0'), static_cast<unsigned>(ymd.month()), "-", std::setw(2), std::setfill('0'), static_cast<unsigned>(ymd.day()), "_", std::setw(2),
		    std::setfill('0'), hms.hours().count(), "-", std::setw(2), std::setfill('0'), hms.minutes().count(), "-", std::setw(2), std::setfill('0'), hms.seconds().count(), "_", std::setw(19), std::setfill('0'), to_uint64_t(tp));
	}
#endif
}  // namespace common