#include <common_output.h>

auto main() -> int {
	if (common::println(1, "2", '3'); common::stringprint(1, "2", '3') != "123") return 1;
#ifdef __cpp_concepts
	if (common::dprintln<", ">(1, "2", '3'); common::dstringprint<", ">(1, "2", '3') != "1, 2, 3") return 1;
	if (common::as_arrayprint(1, "2", '3'); common::abdstringprint<"[", ", ", "]">(1, "2", '3') != "[1, 2, 3]") return 1;
#endif

	{
		std::array a{1, 2, 3};
		common::println(std::array{1, 2, 3});
		common::println(a);
		common::println(std::move(a));
	}

	{
		std::vector a{1, 2, 3};
		common::println(std::vector{1, 2, 3});
		common::println(a);
		common::println(std::move(a));
	}

	{
		std::list a{1, 2, 3};
		common::println(std::list{1, 2, 3});
		common::println(a);
		common::println(std::move(a));
	}

	{
		std::map<std::string, int> a{{"CPU", 1}, {"GPU", 2}, {"RAM", 3}};
		common::println(std::map<std::string, int>{{"CPU", 1}, {"GPU", 2}, {"RAM", 3}});
		common::println(a);
		common::println(std::move(a));
	}

	{
		std::unordered_map<std::string, int> a{{"CPU", 1}, {"GPU", 2}, {"RAM", 3}};
		common::println(std::unordered_map<std::string, int>{{"CPU", 1}, {"GPU", 2}, {"RAM", 3}});
		common::println(a);
		common::println(std::move(a));
	}
}