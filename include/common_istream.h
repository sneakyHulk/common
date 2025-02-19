#pragma once

#include <istream>

namespace common {
	// adapted from Tony Delroy
	struct chlit {
		explicit chlit(char const c) : c_(c) {}
		friend std::istream& operator>>(std::istream& is, chlit const x) {
			if (char c; is >> c && c != x.c_) is.setstate(std::iostream::failbit);

			return is;
		}
		char c_;
	};
}  // namespace common