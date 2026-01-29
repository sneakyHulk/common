#include "common_istream.h"

#include <iostream>
#include <sstream>

auto main() -> int {
	std::istringstream is{R"(-7.16553,45.6726,-35.1623
-7.16553,45.6726,-35.1623
-7.16553,45.6726,-35.1623
-7.16553,45.6726,-35.1623
)"};

	if (float x, y; is >> x >> common::chlit(',') >> y >> common::literal<','>() >> common::ignore_literal<float>()) {
		std::cout << x << "," << y << std::endl;
		if (x != -7.16553f && y != 45.6726f) return EXIT_FAILURE;
	} else {
		return EXIT_FAILURE;
	}

	if (float x, y; is >> x >> common::chlit(',') >> y >> common::literal<','>() >> common::ignore_literal<float>()) {
		std::cout << x << "," << y << std::endl;
		if (x != -7.16553f && y != 45.6726f) return EXIT_FAILURE;
	} else {
		return EXIT_FAILURE;
	}

	if (float x, y; is >> x >> common::chlit(',') >> y >> common::literal<','>() >> common::ignore_literal<float>()) {
		std::cout << x << "," << y << std::endl;
		if (x != -7.16553f && y != 45.6726f) return EXIT_FAILURE;
	} else {
		return EXIT_FAILURE;
	}

	if (float x, y; is >> x >> common::chlit(',') >> y >> common::literal<','>() >> common::ignore_literal<float>()) {
		std::cout << x << "," << y << std::endl;
		if (x != -7.16553f && y != 45.6726f) return EXIT_FAILURE;
	} else {
		return EXIT_FAILURE;
	}
}