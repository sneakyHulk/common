#include <common_output.h>
#include <pyplot.h>

#include <random>
#include <ranges>
#include <vector>

auto main() -> int {
	{
		auto [fig, axs] = plt::instance().subplots(2, 2);

		std::random_device rd{};
		std::mt19937 gen{rd()};

		std::normal_distribution d{0.0, 1.0};
		std::normal_distribution d2{5.0, 1.0};

		std::vector<double> v(100000);
		std::vector<double> v2(100000);
		std::ranges::generate(v, [&d, &gen]() { return d(gen); });
		std::ranges::generate(v2, [&d2, &gen]() { return d2(gen); });

		auto im = axs[0].hist2d(Args(v, v2), Kwargs("bins"_a = 40, "norm"_a = "log"));
		fig.colorbar(Args(plt::at(im.unwrap(), 3)), Kwargs("ax"_a = axs[0].unwrap()));

		axs[1].plot(Args(std::vector<int>({1, 3, 2, 4})), Kwargs("color"_a = "blue", "linewidth"_a = 1.0));
		axs[2].scatter(Args(std::vector<int>({1, 3, 2, 4}), std::vector<int>({1, 2, 3, 4})), Kwargs("color"_a = "blue"));

		plt::instance().pause(Args(2), Kwargs());
	}
	{
		auto fig = plt::instance().figure(Args(), Kwargs());
		auto gs = matplotlibcpp17::gridspec::GridSpec(3, 2);
		auto ax1 = fig.add_subplot(Args(gs(2, py::slice(0, 2, 1)).unwrap()));
		auto ax2 = fig.add_subplot(Args(gs(0, py::slice(0, 2, 1)).unwrap()));

		ax1.plot(Args(std::vector<int>({1, 2, 3, 4}), std::vector<int>({1, 3, 2, 400})), Kwargs("color"_a = "blue"));
		ax1.set_yscale(Args("log"));

		ax2.plot(Args(std::vector<int>({1, 2, 3, 4}), std::vector<int>({1, 3, 2, 400})), Kwargs("color"_a = "blue"));
		ax2.set_yscale(Args("linear"));

		plt::instance().pause(Args(2), Kwargs());
	}
}