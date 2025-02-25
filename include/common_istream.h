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

	template <auto lit>
	struct literal {
		friend std::istream& operator>>(std::istream& is, literal<lit> const x) {
			if (decltype(lit) value; is >> value && value != lit) is.setstate(std::iostream::failbit);

			return is;
		}
	};

	template <typename T>
	struct ignore_literal {
		friend std::istream& operator>>(std::istream& is, ignore_literal<T> const x) {
			if (T value; is >> value) is.setstate(std::iostream::failbit);

			return is;
		}
	};
}  // namespace common