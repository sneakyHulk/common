#include "common_error.h"

#include <ostream>

common::Error::Error(int const code, std::string message) : code(code), message(std::move(message)) {}
common::Error::Error(int const code) : code(code), message(std::to_string(code)) {}
common::Error::Error(std::string message) : code(0), message(std::move(message)) {}
char const* common::Error::what() const noexcept { return message.c_str(); }

std::ostream& common::operator<<(std::ostream& os, common::Error const& err) {
	os << err.message << " (" << err.code << ")";

	return os;
}