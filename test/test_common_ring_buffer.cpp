#include <cstdint>
#include <cstdlib>
#include <deque>
#include <random>

#include "common_ring_buffer.h"

template <typename T, std::size_t N>
static bool equal(common::ring_buffer<T, N> const& rb, std::deque<T> const& model) {
	if (rb.size() != model.size() || rb.empty() != model.empty() || rb.full() != (model.size() == N)) return false;
	for (std::size_t i = 0; i < model.size(); ++i)
		if (rb[i] != model[i]) return false;
	return true;
}

auto main() -> int {
	// Test 1: push_back, size, front, back
	{
		common::ring_buffer<std::uint8_t, 4> rb;
		if (!rb.empty() || rb.writable().size() != 4) return EXIT_FAILURE;

		rb.push_back(1);
		rb.push_back(2);
		rb.push_back(3);

		if (rb.size() != 3) return EXIT_FAILURE;
		if (rb[0] != 1) return EXIT_FAILURE;
		if (rb.back() != 3) return EXIT_FAILURE;
	}

	// Test 2: wrap-around overwrite
	{
		common::ring_buffer<std::uint8_t, 3> rb;

		rb.push_back(10);
		rb.push_back(20);
		rb.push_back(30);
		rb.push_back(40);  // overwrite 10

		if (!equal(rb, std::deque<std::uint8_t>{20, 30, 40})) return EXIT_FAILURE;
	}

	// Test 3: pop behavior, clamps to size()
	{
		common::ring_buffer<std::uint8_t, 5> rb;
		for (std::uint8_t v : {1, 2, 3, 4}) rb.push_back(v);

		rb.pop();
		rb.pop();
		if (!equal(rb, std::deque<std::uint8_t>{3, 4})) return EXIT_FAILURE;

		rb.pop(100);
		if (!rb.empty() || rb.writable().size() != 5) return EXIT_FAILURE;
	}

	// Test 4: writable() / commit() wrapping in two parts
	{
		common::ring_buffer<std::uint8_t, 8> rb;
		for (int i = 0; i < 6; ++i) rb.push_back(i);
		rb.pop(4);

		auto first = rb.writable();
		if (first.size() != 2) return EXIT_FAILURE;
		first[0] = 6;
		first[1] = 7;
		rb.commit(2);

		auto second = rb.writable();
		if (second.size() != 4) return EXIT_FAILURE;
		for (std::size_t k = 0; k < second.size(); ++k) second[k] = static_cast<std::uint8_t>(8 + k);
		rb.commit(second.size());

		if (!rb.full() || !rb.writable().empty()) return EXIT_FAILURE;
		for (std::size_t i = 0; i < rb.size(); ++i)
			if (rb[i] != 4 + i) return EXIT_FAILURE;
	}

	// Test 5: random single bytes, chunks via writable() (like read_some) and pops against std::deque
	{
		common::ring_buffer<std::uint8_t, 16> rb;
		std::deque<std::uint8_t> model;
		std::mt19937 rng{};
		std::uint8_t next = 0;

		for (int i = 0; i < 10000; ++i) {
			switch (rng() % 3) {
				case 0: {
					rb.push_back(next);
					model.push_back(next++);
					if (model.size() > 16) model.pop_front();
					break;
				}
				case 1: {
					auto const free = rb.writable();
					auto const n = free.empty() ? 0 : rng() % (free.size() + 1);
					for (std::size_t k = 0; k < n; ++k) {
						free[k] = next;
						model.push_back(next++);
					}
					rb.commit(n);
					break;
				}
				default: {
					auto const n = rng() % 10;
					rb.pop(n);
					for (std::size_t k = 0; k < n && !model.empty(); ++k) model.pop_front();
				}
			}
			if (!equal(rb, model)) return EXIT_FAILURE;
		}
	}

	// Test 6: writable() is empty when full, commit(0) changes nothing
	{
		common::ring_buffer<std::uint8_t, 4> rb;
		for (std::uint8_t v : {1, 2, 3, 4}) rb.push_back(v);
		if (!rb.writable().empty()) return EXIT_FAILURE;
		rb.commit(0);
		if (!equal(rb, std::deque<std::uint8_t>{1, 2, 3, 4})) return EXIT_FAILURE;
	}

	// Test 7: full buffer, pop from the front -> the free space is at the physical start
	{
		common::ring_buffer<std::uint8_t, 8> rb;
		for (int i = 0; i < 8; ++i) rb.push_back(i);
		rb.pop(3);

		auto const free = rb.writable();
		if (free.size() != 3) return EXIT_FAILURE;
		for (std::size_t k = 0; k < free.size(); ++k) free[k] = static_cast<std::uint8_t>(8 + k);
		rb.commit(free.size());

		if (!rb.full()) return EXIT_FAILURE;
		for (std::size_t i = 0; i < rb.size(); ++i)
			if (rb[i] != 3 + i) return EXIT_FAILURE;
	}

	// Test 8: writable() never points at unread elements and is only empty when full
	{
		common::ring_buffer<std::uint8_t, 13> rb;
		std::mt19937 rng{};

		for (int i = 0; i < 10000; ++i) {
			auto const free = rb.writable();
			if (rb.full() != free.empty()) return EXIT_FAILURE;
			if (free.size() > rb.capacity() - rb.size()) return EXIT_FAILURE;

			for (std::size_t n = 0; n < rb.size(); ++n) {
				if (auto const* const element = &rb[n]; element >= free.data() && element < free.data() + free.size()) return EXIT_FAILURE;
			}

			if (rng() % 2)
				rb.commit(free.empty() ? 0 : rng() % (free.size() + 1));
			else
				rb.pop(rng() % 5);
		}
	}

	return EXIT_SUCCESS;
}
