#pragma once

#include <cstdlib>
#include <source_location>
#include <string_view>

#include "common.h"
#include "common_output_backend.h"

#ifndef ARDUINO
#include <sstream>

#include "common_exception.h"
#endif

#ifdef __cpp_concepts
#include <concepts>
#endif

namespace common {
	namespace detail {
		// file name without directory and extension, like std::filesystem::path::stem()
		constexpr std::string_view stem(std::string_view path) {
			path = path.substr(path.find_last_of("/\\") + 1);
			return path.substr(0, path.find_last_of('.'));
		}

		inline void write_location(std::source_location const& location) {
			backend::write('[');
			backend::write(stem(location.file_name()));
			backend::write("]: ");
		}
	}  // namespace detail

#ifndef ARDUINO  // returns std::string, host only
#ifdef __cpp_concepts
	[[maybe_unused]] std::string stringprint(printable auto&&... args) {
#else
	template <typename... T>
	[[maybe_unused]] std::string stringprint(T&&... args) {
#endif
		std::ostringstream ss;
		(ss << ... << std::forward<decltype(args)>(args));
		return ss.str();
	}

#ifdef __cpp_concepts
	template <StringLiteral delimiter = " ">
	[[maybe_unused]] std::string dstringprint(printable auto&&... args) {
		constexpr auto delim = delimiter.value;
		std::ostringstream ss;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&delim, &ss]<printable... T0>(auto& println_ref, printable auto&& first, T0&&... args) -> void {
				ss << first;
				if constexpr (sizeof...(args)) {
					ss << delim;
					println_ref(println_ref, std::forward<T0>(args)...);
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
			auto println_recursive = [&d, &ss]<printable... T0>(auto& println_ref, printable auto&& first, T0&&... args) -> void {
				ss << first;
				if constexpr (sizeof...(args)) {
					ss << d;
					println_ref(println_ref, std::forward<T0>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		ss << e;

		return ss.str();
	}
#endif
#endif

#ifdef __cpp_concepts
	template <printable... Args>
	struct print_debug_loc {
		explicit print_debug_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			detail::write_location(location);
			(backend::write(args), ...);
			backend::flush();
		}
	};

	template <printable... Args>
	print_debug_loc(Args&&... args) -> print_debug_loc<Args...>;

	void print_debug(printable auto&&... args) {
		(backend::write(args), ...);
		backend::flush();
	}

	template <printable... Args>
	struct print_loc {
		explicit print_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			detail::write_location(location);
			(backend::write(args), ...);
			backend::flush();
		}
	};

	template <printable... Args>
	print_loc(Args&&... args) -> print_loc<Args...>;

	[[maybe_unused]] void print(printable auto&&... args) {
#else
	template <typename... T>
	[[maybe_unused]] void print(T&&... args) {
#endif
		(backend::write(args), ...);
		backend::flush();
	}

#ifdef __cpp_concepts
	template <StringLiteral delimiter = " ">
	[[maybe_unused]] void dprint(printable auto&&... args) {
		constexpr auto delim = delimiter.value;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&delim]<printable... T0>(auto& println_ref, printable auto&& first, T0&&... args) -> void {
				backend::write(first);
				if constexpr (sizeof...(args)) {
					backend::write(delim);
					println_ref(println_ref, std::forward<T0>(args)...);
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

		backend::write(s);

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&d]<printable... T0>(auto& println_ref, printable auto&& first, T0&&... args) -> void {
				backend::write(first);
				if constexpr (sizeof...(args)) {
					backend::write(d);
					println_ref(println_ref, std::forward<T0>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		backend::write(e);
	}
#endif
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
			detail::write_location(location);
			(backend::write(args), ...);
			backend::newline();
		}
	};

	template <printable... Args>
	println_loc(Args&&... args) -> println_loc<Args...>;

	template <printable... Args>
	struct println_debug_loc {
		explicit println_debug_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			detail::write_location(location);
			(backend::write(args), ...);
			backend::newline();
		}
	};

	template <printable... Args>
	println_debug_loc(Args&&... args) -> println_debug_loc<Args...>;

	void println_debug(printable auto&&... args) {
		(backend::write(args), ...);
		backend::newline();
	}

	template <printable... Args>
	struct println_warn_loc {
		explicit println_warn_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			backend::write(YEL);
			detail::write_location(location);
			(backend::write(args), ...);
			backend::write(RESET);
			backend::newline();
		}
	};

	template <printable... Args>
	println_warn_loc(Args&&... args) -> println_warn_loc<Args...>;

	void println_warn(printable auto&&... args) {
		backend::write(YEL);
		(backend::write(args), ...);
		backend::write(RESET);
		backend::newline();
	}

	template <printable... Args>
	struct println_error_loc {
		explicit println_error_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			backend::write(RED);
			detail::write_location(location);
			(backend::write(args), ...);
			backend::write(RESET);
			backend::newline();
		}
	};

	template <printable... Args>
	println_error_loc(Args&&... args) -> println_error_loc<Args...>;

	void println_error(printable auto&&... args) {
		backend::write(RED);
		(backend::write(args), ...);
		backend::write(RESET);
		backend::newline();
	}

	// throws common::Exception on the host, stops the program where exceptions are not available
	template <printable... Args>
	struct println_critical_loc {
		explicit println_critical_loc(Args&&... args, std::source_location const location = std::source_location::current()) {
			backend::write(RED);
			detail::write_location(location);
			(backend::write(args), ...);
			backend::write(RESET);
			backend::newline();
#if !defined(ARDUINO) && defined(__cpp_exceptions)
			throw Exception(std::forward<Args>(args)...);
#else
			backend::flush();
			std::abort();
#endif
		}
	};

	template <printable... Args>
	println_critical_loc(Args&&... args) -> println_critical_loc<Args...>;

	void println_critical(printable auto&&... args) {
		backend::write(RED);
		(backend::write(args), ...);
		backend::write(RESET);
		backend::newline();
#if !defined(ARDUINO) && defined(__cpp_exceptions)
		throw Exception(std::forward<decltype(args)>(args)...);
#else
		backend::flush();
		std::abort();
#endif
	}

	[[maybe_unused]] void println(printable auto&&... args) {
#else
	template <typename... T>
	[[maybe_unused]] void println(T&&... args) {
#endif
		(backend::write(args), ...);
		backend::newline();
	}

#ifdef __cpp_concepts
	template <StringLiteral delimiter = " ">
	[[maybe_unused]] void dprintln(printable auto&&... args) {
		constexpr auto delim = delimiter.value;

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&delim]<printable... T0>(auto& println_ref, printable auto&& first, T0&&... args) -> void {
				backend::write(first);
				if constexpr (sizeof...(args)) {
					backend::write(delim);
					println_ref(println_ref, std::forward<T0>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		backend::newline();
	}
#endif

#ifdef __cpp_concepts
	template <StringLiteral start, StringLiteral delimiter, StringLiteral end>
	[[maybe_unused]] void abdprintln(printable auto&&... args) {
		constexpr auto d = delimiter.value;
		constexpr auto s = start.value;
		constexpr auto e = end.value;

		backend::write(s);

		if constexpr (sizeof...(args)) {
			auto println_recursive = [&d]<printable... T0>(auto& println_ref, printable auto&& first, T0&&... args) -> void {
				backend::write(first);
				if constexpr (sizeof...(args)) {
					backend::write(d);
					println_ref(println_ref, std::forward<T0>(args)...);
				}
			};

			println_recursive(println_recursive, std::forward<decltype(args)>(args)...);
		}

		backend::write(e);
		backend::newline();
	}
#endif

#ifdef __cpp_concepts
	[[maybe_unused]] void as_arrayprint(printable auto&&... args) { return abdprintln<"[", ", ", "]">(std::forward<decltype(args)>(args)...); }
#endif
}  // namespace common

#if defined(__cpp_lib_format) && !defined(ARDUINO)
#include <format>
namespace common {
	constexpr std::string to_bin(std::integral auto num) {
		std::stringstream ss;
		ss << std::format("{:b}", num);
		return ss.str();
	}

}  // namespace common
#endif
