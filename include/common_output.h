#pragma once

#include <common.h>
#include <common_exception.h>
#include <common_ostream.h>

#include <filesystem>
#include <iostream>
#include <source_location>
#include <sstream>

namespace common {

#ifdef __cpp_concepts
	[[maybe_unused]] std::string stringprint(printable auto&&... args) {
#else
	template <typename... T>
	[[maybe_unused]] std::string stringprint(T&&... args) {
#endif
		std::ostringstream ss;
		(ss << ... << args);
		return ss.str();
	}

#ifdef __cpp_concepts
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
#endif

#ifdef __cpp_concepts
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
#endif

#ifdef __cpp_concepts
	template <printable... Args>
	struct print_loc {
		explicit print_loc(Args&&... args, std::source_location const location = std::source_location::current()) { ((std::cout << '[' << std::filesystem::path(location.file_name()).stem().string() << "]: ") << ... << args) << std::flush; }
	};

	template <printable... Args>
	print_loc(Args&&... args) -> print_loc<Args...>;

	[[maybe_unused]] void print(printable auto&&... args) {
#else
	template <typename... T>
	[[maybe_unused]] void print(T&&... args) {
#endif
		(std::cout << ... << args) << std::flush;
	}

#ifdef __cpp_concepts
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
#endif

#ifdef __cpp_concepts
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
#endif
	enum class LOGLEVEL {
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		CRITICAL,
	};

#define BLK "\033[0;30m"
#define RED "\033[0;31m"
#define GRN "\033e[0;32m"
#define YEL "\033[0;33m"
#define BLU "\033[0;34m"
#define MAG "\033[0;35m"
#define CYN "\033[0;36m"
#define WHT "\033[0;37m"
#define RESET "\033[0m"

#ifdef __cpp_concepts
	template <printable... Args>
	struct println_loc {
		explicit println_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			((std::cout << '[' << std::filesystem::path(location.file_name()).stem().string() << "]: ") << ... << std::forward<Args>(args)) << std::endl;
		}
	};

	template <printable... Args>
	println_loc(Args&&... args) -> println_loc<Args...>;

	template <printable... Args>
	struct println_warn_loc {
		explicit println_warn_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			((std::cout << YEL << '[' << std::filesystem::path(location.file_name()).stem().string() << "]: ") << ... << std::forward<Args>(args)) << RESET << std::endl;
		}
	};

	template <printable... Args>
	println_warn_loc(Args&&... args) -> println_warn_loc<Args...>;

	void println_warn(printable auto&&... args) { ((std::cout << YEL) << ... << args) << RESET << std::endl; }

	template <printable... Args>
	struct println_error_loc {
		explicit println_error_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			((std::cout << RED << '[' << std::filesystem::path(location.file_name()).stem().string() << "]: ") << ... << std::forward<Args>(args)) << RESET << std::endl;
		}
	};

	template <printable... Args>
	println_error_loc(Args&&... args) -> println_error_loc<Args...>;

	void println_error(printable auto&&... args) { ((std::cout << RED) << ... << std::forward<decltype(args)>(args)) << RESET << std::endl; }

	template <printable... Args>
	struct println_critical_loc {
		explicit println_critical_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			((std::cout << RED << '[' << std::filesystem::path(location.file_name()).stem().string() << "]: ") << ... << std::forward<Args>(args)) << RESET << std::endl;
			throw Exception(std::forward<Args>(args)...);
		}
	};

	template <printable... Args>
	println_critical_loc(Args&&... args) -> println_critical_loc<Args...>;

	void println_critical(printable auto&&... args) {
		((std::cout << RED) << ... << args) << RESET << std::endl;
		throw Exception(std::forward<decltype(args)>(args)...);
	}

	[[maybe_unused]] void println(printable auto&&... args) {
#else
	template <typename... T>
	[[maybe_unused]] void println(T&&... args) {
#endif
		(std::cout << ... << args) << std::endl;
	}

#ifdef __cpp_concepts
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
#endif

#ifdef __cpp_concepts
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
#endif

#ifdef __cpp_concepts
	[[maybe_unused]] void as_arrayprint(printable auto&&... args) { return abdprintln<"[", ", ", "]">(std::forward<decltype(args)>(args)...); }
#endif
}  // namespace common

#ifdef __cpp_lib_format
#include <format>
namespace common {
	constexpr std::string to_bin(std::integral auto num) {
		std::stringstream ss;
		ss << std::format("{:b}", num);
		return ss.str();
	}

}  // namespace common
#endif