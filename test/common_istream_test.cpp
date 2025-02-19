#include "common_istream.h"

#include <iostream>
#include <sstream>

auto main() -> int {
	std::istringstream is{R"(-7.16553,45.6726,-35.1623
-7.16553,45.6726,-35.1623
-7.16553,45.6726,-35.1623
-7.16553,45.6726,-35.1623
)"};

	float x, y, z;
	while (is >> x >> common::chlit(',') >> y >> common::chlit(',') >> z) {
		std::cout << x << "," << y << "," << z << std::endl;
		if (x != -7.16553f && y != 45.6726f && z != -35.1623f) return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}