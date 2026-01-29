#include <common_error.h>

#include <sstream>

auto main() -> int {
	if ((std::ostringstream{} << common::Error{404, "Not Found"}).str() != "Not Found (404)") return EXIT_FAILURE;
	if ((std::ostringstream{} << common::Error{"Failure"}).str() != "Failure (0)") return EXIT_FAILURE;

	return EXIT_SUCCESS;
}