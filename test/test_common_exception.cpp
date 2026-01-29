#include "common_exception.h"

#include <cstring>
#include <iostream>

auto main() -> int {
	try {
		throw common::Exception("hi ", "test");
	} catch (common::Exception const& e) {
		std::cout << e.what();
		if (std::strcmp(e.what(), "hi test") == 0) {
			return EXIT_SUCCESS;
		}
	}

	return EXIT_FAILURE;
}