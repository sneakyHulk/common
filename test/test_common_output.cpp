#include <common_output.h>

auto main() -> int {
	if (common::println(1, "2", '3'); common::stringprint(1, "2", '3') != "123") return EXIT_FAILURE;
#ifdef __cpp_concepts
	if (common::dprintln<", ">(1, "2", '3'); common::dstringprint<", ">(1, "2", '3') != "1, 2, 3") return EXIT_FAILURE;
	if (common::as_arrayprint(1, "2", '3'); common::abdstringprint<"[", ", ", "]">(1, "2", '3') != "[1, 2, 3]") return EXIT_FAILURE;
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

	{
		std::tuple<double, double, int> a{1., 2.5, 3};
		common::println(std::tuple<double, double, int>{1., 2.5, 3});
		common::println(a);
		common::println(std::move(a));
	}

	{
		std::pair<double, int> a{1.5, 2};
		common::println(std::pair<double, int>{1.5, 2});
		common::println(a);
		common::println(std::move(a));
	}

	{
		std::set<double> a{1.5, 2.9, 6.3};
		common::println(std::set<double>{1.5, 2.9, 6.3});
		common::println(a);
		common::println(std::move(a));
	}

	return EXIT_SUCCESS;
}