#include "common_exception.h"

auto main() -> int {
	try {
		throw common::Exception("hi ", "test");
	} catch (common::Exception const& e) {
		common::println(e.what());
		return 0;
	}

	return 1;
}