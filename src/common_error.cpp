#include "common_error.h"

#ifndef ARDUINO
common::Error::Error(int const code, std::string_view const text) : code(code), message(text) {}
common::Error::Error(int const code) : code(code), message(std::to_string(code)) {}
common::Error::Error(std::string_view const text) : Error(0, text) {}
char const* common::Error::what() const noexcept { return message.c_str(); }

std::ostream& common::operator<<(std::ostream& os, common::Error const& err) {
	os << err.message << " (" << err.code << ")";

	return os;
}
#endif