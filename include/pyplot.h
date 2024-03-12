#include <common.h>
#include <matplotlibcpp17/pyplot.h>

#pragma GCC visibility push(hidden)
class [[maybe_unused]] plt : private py::scoped_interpreter, public matplotlibcpp17::pyplot::PyPlot {
#pragma GCC visibility pop
   public:
	[[maybe_unused]] static plt& instance() {
		static plt instance;
		return instance;
	}
	[[maybe_unused]] static auto& at(common::iterable auto&& pyobj, std::integral auto index) {
		for (auto i = 0; auto& e : pyobj) {
			if (i++ == index) {
				return e;
			}
		}
		throw std::out_of_range("Index of pyobj out of range!");
	}
	[[maybe_unused]] static auto& at(common::iterable auto const& pyobj, std::integral auto index) {
		for (auto i = 0; auto& e : pyobj) {
			if (i++ == index) {
				return e;
			}
		}
		throw std::out_of_range("Index of pyobj out of range!");
	}

   private:
	[[maybe_unused]] plt() : matplotlibcpp17::pyplot::PyPlot(matplotlibcpp17::pyplot::import()) {}

   public:
	plt(plt const&) = delete;
	void operator=(plt const&) = delete;
};