#include <EigenJsonUtils.h>
#include <common_output.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <ranges>
#include <vector>

int main() {
	Eigen::VectorXd v(5);
	v << 1, std::numeric_limits<double>::quiet_NaN(), 3, std::numeric_limits<double>::quiet_NaN(), 5;

	Eigen::Vector<bool, -1> mask = !v.array().isNaN();

	std::cout << mask.count() << std::endl;

	for (auto const [value, filter] : std::ranges::views::zip(v, mask) | std::ranges::views::filter([](auto const &v) { return !std::get<1>(v); })) {
		std::cout << value << std::endl;
	}

	auto filtered = std::views::zip(v, mask) | std::views::filter([](auto pair) { return std::get<1>(pair); }) | std::views::transform([](auto pair) { return std::get<0>(pair); });

	for (auto i = 0; auto const j :
	    std::ranges::views::zip(mask, std::ranges::views::iota(0)) | std::ranges::views::filter([](auto const &pair) { return std::get<0>(pair); }) | std::ranges::views::transform([](auto const &pair) { return std::get<1>(pair); })) {
		std::cout << i++ << ", " << j << std::endl;
	}

	std::cout << v << std::endl;

	// Eigen + Json
	nlohmann::json current_calibration_json;
	for (auto i = 0; i < 5; ++i) {
		Eigen::Matrix<double, 4, 4> out = Eigen::Matrix<double, 4, 4>::Identity();

		std::stringstream name;

		name << std::setw(2) << std::setfill('0') << i;
		current_calibration_json[name.str()]["transformation"] = out;
	}

	std::vector transformations =
	    current_calibration_json | std::ranges::views::transform([](nlohmann::basic_json<> const &value) { return value["transformation"].template get<Eigen::Matrix<double, 4, 4>>(); }) | std::ranges::to<std::vector>();

	common::println(transformations);
}