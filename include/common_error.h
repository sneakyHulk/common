#pragma once

#ifdef ARDUINO
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace common {
	// Arduino: no heap, the message is a fixed char array, longer messages are cut; only positive codes
	struct Error {
		static constexpr std::size_t max_message_size = 63;

		std::uint32_t code;
		std::array<char, max_message_size + 1> message{};

		Error(std::uint32_t const code, std::string_view const text) : code(code) {
			auto const size = std::min(text.size(), max_message_size);
			std::copy_n(text.data(), size, message.data());
			message[size] = '\0';
		}

		// message = the decimal number of code
		explicit Error(std::uint32_t const code) : code(code) {
			std::array<char, 10> digits{};  // at most 10 digits, written backwards
			std::size_t n = 0;
			for (std::uint32_t value = code; n == 0 || value > 0; value /= 10) digits[n++] = static_cast<char>('0' + value % 10);

			std::size_t i = 0;
			while (n > 0) message[i++] = digits[--n];
			message[i] = '\0';
		}

		explicit Error(std::string_view const text) : Error(0, text) {}

		[[nodiscard]] char const* what() const noexcept { return message.data(); }
	};
}  // namespace common
#else
#include <ostream>
#include <string>
#include <string_view>

namespace common {
	struct Error {
		int code;
		std::string message;

		Error(int code, std::string_view text);
		explicit Error(int code);
		explicit Error(std::string_view text);

		[[nodiscard]] char const* what() const noexcept;
	};

	std::ostream& operator<<(std::ostream& os, Error const& err);
}  // namespace common
#endif
