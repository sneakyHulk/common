#pragma once

#include <ostream>
#include <string>

namespace common {
	struct Error {
		int code;
		std::string message;

		Error(int code, std::string message);
		explicit Error(int code);
		explicit Error(std::string message);

		~Error() = default;

		[[nodiscard]] char const* what() const noexcept;
	};

	std::ostream& operator<<(std::ostream& os, Error const& err);
}  // namespace common
