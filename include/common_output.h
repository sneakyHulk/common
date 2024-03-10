#pragma once

#include <common.h>
#include <common_ostream.h>

#include <format>
#include <iostream>
#include <sstream>

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

	template <StringLiteral start, StringLiteral delimiter, StringLiteral end>
	[[maybe_unused]] std::string abdstringprint(printable auto&&... args) {
		constexpr auto d = delimiter.value;
		constexpr auto s = start.value;
		constexpr auto e = end.value;
		std::ostringstream ss;

		ss << s;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&d, &ss](auto& println_ref, printable auto&& first, printable auto&&... args) -> void {
				ss << first;
				if constexpr (sizeof...(args)) {
					ss << d;
					println_ref(println_ref, std::forward<decltype(args)>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		ss << e;

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

	template <StringLiteral start, StringLiteral delimiter, StringLiteral end>
	[[maybe_unused]] void abdprint(printable auto&&... args) {
		constexpr auto d = delimiter.value;
		constexpr auto s = start.value;
		constexpr auto e = end.value;

		std::cout << s;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&d](auto& println_ref, printable auto&& first, printable auto&&... args) -> void {
				std::cout << first;
				if constexpr (sizeof...(args)) {
					std::cout << d;
					println_ref(println_ref, std::forward<decltype(args)>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		std::cout << e;
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

	template <StringLiteral start, StringLiteral delimiter, StringLiteral end>
	[[maybe_unused]] void abdprintln(printable auto&&... args) {
		constexpr auto d = delimiter.value;
		constexpr auto s = start.value;
		constexpr auto e = end.value;

		std::cout << s;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&d](auto& println_ref, printable auto&& first, printable auto&&... args) -> void {
				std::cout << first;
				if constexpr (sizeof...(args)) {
					std::cout << d;
					println_ref(println_ref, std::forward<decltype(args)>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		std::cout << e << std::endl;
	}

	[[maybe_unused]] void as_arrayprint(printable auto&&... args) { return abdprintln<"[", ", ", "]">(std::forward<decltype(args)>(args)...); }

	constexpr std::string to_bin(std::integral auto num) {
		std::stringstream ss;
		ss << std::format("{:b}", num);
		return ss.str();
	}
}  // namespace common