#include "common_ring_buffer.h"

auto main() -> int {
	// Test 1: push_back, size, front, back
	{
		common::ring_buffer<std::uint8_t, 4> rb;

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

		if (rb.size() != 3) return EXIT_FAILURE;
		if (rb.front() != 20) return EXIT_FAILURE;
		if (rb[1] != 30) return EXIT_FAILURE;
		if (rb.back() != 40) return EXIT_FAILURE;
	}

	// Test 3: pop behavior
	{
		common::ring_buffer<std::uint8_t, 5> rb;

		rb.push_back(1);
		rb.push_back(2);
		rb.push_back(3);
		rb.push_back(4);

		rb.pop();
		rb.pop();

		if (rb.size() != 2) return EXIT_FAILURE;
		if (rb.front() != 3) return EXIT_FAILURE;
		if (rb.back() != 4) return EXIT_FAILURE;
	}

	// Test 4: linear_sub_array()
	{
		common::ring_buffer<std::uint8_t, 9> rb;

		rb.push_back(1);
		rb.push_back(2);
		rb.push_back(3);

		auto span = rb.linear_sub_array<4>();
		if (span.size() != 4) return EXIT_FAILURE;

		std::array<std::uint8_t, 3> const arr{1, 2, 3};
		std::memcpy(span.data(), arr.begin(), arr.size());
		rb.rotate(arr.size());

		if (rb[3] != 1) return EXIT_FAILURE;
		if (rb[4] != 2) return EXIT_FAILURE;
		if (rb[5] != 3) return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}