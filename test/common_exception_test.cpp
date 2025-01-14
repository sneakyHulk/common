#include "common_exception.h"

#include "common_output.h"

auto main() -> int {
	try {
		throw common::Exception("hi ", "test");
	} catch (common::Exception const& e) {
		common::println(e.what());
		return 0;
	}

	return 1;
}