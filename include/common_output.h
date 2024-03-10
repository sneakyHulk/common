#pragma once

#include <common.h>

#include <iostream>
#include <sstream>
#include <array>
#include <vector>

namespace common {
	template <typename T>
	concept printable = requires(std::ostream& os, const T& msg) {
		{ os << msg };
	};

	[[maybe_unused]] std::string stringprint(printable auto&&... args) {
		std::ostringstream ss;
		(ss << ... << args);
		return ss.str();
	}

	template <StringLiteral delimiter = " ">
	[[maybe_unused]] std::string dstringprint(printable auto&&... args) {
		constexpr auto delim = delimiter.value;
		std::ostringstream ss;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&delim, &ss](auto& println_ref, printable auto&& first, printable auto&&... args) -> void {
				ss << first;
				if constexpr (sizeof...(args)) {
					ss << delim;
					println_ref(println_ref, std::forward<decltype(args)>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		return ss.str();
	}

	[[maybe_unused]] void print(printable auto&&... args) {
		std::ostringstream ss;
		(std::cout << ... << args) << std::flush;
	}

	template <StringLiteral delimiter = " ">
	[[maybe_unused]] void dprint(printable auto&&... args) {
		constexpr auto delim = delimiter.value;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&delim](auto& println_ref, printable auto&& first, printable auto&&... args) -> void {
				std::cout << first;
				if constexpr (sizeof...(args)) {
					std::cout << delim;
					println_ref(println_ref, std::forward<decltype(args)>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}
	}

	[[maybe_unused]] void println(printable auto&&... args) {
		std::ostringstream ss;
		(std::cout << ... << args) << std::endl;
	}

	template <StringLiteral delimiter = " ">
	[[maybe_unused]] void dprintln(printable auto&&... args) {
		constexpr auto delim = delimiter.value;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&delim](auto& println_ref, printable auto&& first, printable auto&&... args) -> void {
				std::cout << first;
				if constexpr (sizeof...(args)) {
					std::cout << delim;
					println_ref(println_ref, std::forward<decltype(args)>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		std::cout << std::endl;
	}
}  // namespace common