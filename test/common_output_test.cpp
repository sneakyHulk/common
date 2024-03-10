#include <common_output.h>

auto main() -> int {
	if (common::println(1, "2", '3'); common::stringprint(1, "2", '3') != "123") return 1;
	if (common::dprintln<", ">(1, "2", '3'); common::dstringprint<", ">(1, "2", '3') != "1, 2, 3") return 1;
	if (common::arrayprint(1, "2", '3'); common::abdstringprint<"[", ", ", "]">(1, "2", '3') != "[1, 2, 3]") return 1;
}