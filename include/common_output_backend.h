#pragma once

// Where common::print... writes to: Serial on Arduino, std::cout everywhere else.
// common_output.h only uses backend::write(), backend::newline() and backend::flush().

#ifdef ARDUINO
#include <Arduino.h>

#include <array>
#include <concepts>
#include <cstddef>
#include <string_view>

namespace common::backend {
	inline void write(char const* const value) { Serial.print(value); }
	inline void write(std::string_view const value) { Serial.write(value.data(), value.size()); }
	inline void write(char const value) { Serial.print(value); }
	inline void write(bool const value) { Serial.print(static_cast<int>(value)); }  // like std::cout: 0 / 1
	void write(std::integral auto const value) { Serial.print(value); }
	void write(std::floating_point auto const value) { Serial.print(value, 6); }

	// like operator<< in common_ostream.h: [1, 2, 3]
	template <typename T, std::size_t N>
	void write(std::array<T, N> const& values) {
		write('[');
		for (std::size_t i = 0; i < N; ++i) {
			if (i) write(", ");
			write(values[i]);
		}
		write(']');
	}

	inline void newline() { Serial.println(); }
	inline void flush() { Serial.flush(); }
}  // namespace common::backend

#ifdef __cpp_concepts
template <typename T>
concept printable = requires(T const& value) { common::backend::write(value); };
#endif

#else
#include <iostream>

#include "common_ostream.h"  // operator<< for the std containers and the concept printable

namespace common::backend {
	void write(printable auto const& value) { std::cout << value; }

	inline void newline() { std::cout << std::endl; }
	inline void flush() { std::cout << std::flush; }
}  // namespace common::backend
#endif
