#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <span>

namespace common {
	// Fixed size FIFO without heap. push_back() overwrites the oldest element when full.
	// Zero copy input (DMA, boost::asio read_some): write into writable(), then commit() the number of written elements.
	template <typename T, std::size_t N>
	class ring_buffer {
		std::array<T, N> data{};
		std::size_t head = 0;  // physical index of the oldest element
		std::size_t count = 0;

		[[nodiscard]] static constexpr std::size_t wrap(std::size_t const i) { return i % N; }
		[[nodiscard]] constexpr std::size_t tail() const { return wrap(head + count); }  // physical index of the next write

	   public:
		[[nodiscard]] static constexpr std::size_t capacity() { return N; }
		[[nodiscard]] constexpr std::size_t size() const { return count; }
		[[nodiscard]] constexpr bool empty() const { return count == 0; }
		[[nodiscard]] constexpr bool full() const { return count == N; }

		// n = 0 is the oldest element
		[[nodiscard]] constexpr T &operator[](std::size_t const n) {
			assert(n < count);
			return data[wrap(head + n)];
		}
		[[nodiscard]] constexpr T const &operator[](std::size_t const n) const {
			assert(n < count);
			return data[wrap(head + n)];
		}

		[[nodiscard]] constexpr T &front() { return operator[](0); }
		[[nodiscard]] constexpr T const &front() const { return operator[](0); }
		[[nodiscard]] constexpr T &back() { return operator[](count - 1); }
		[[nodiscard]] constexpr T const &back() const { return operator[](count - 1); }

		constexpr void push_back(T const &value) {
			data[tail()] = value;
			if (full())
				head = wrap(head + 1);
			else
				++count;
		}

		// removes the n oldest elements, at most size()
		constexpr void pop(std::size_t const n = 1) {
			auto const m = std::min(n, count);
			head = wrap(head + m);
			count -= m;
			if (count == 0) head = 0;  // maximizes the next writable()
		}

		// contiguous free space at the write position, never overwrites elements, empty when full
		// can be shorter than capacity() - size() when the free space wraps around
		[[nodiscard]] constexpr std::span<T> writable() {
			auto const begin = tail();
			return {data.data() + begin, std::min(N - count, N - begin)};
		}

		// adds the first n elements of writable() to the buffer
		constexpr void commit(std::size_t const n) {
			assert(n <= writable().size());
			count += n;
		}

		// physical storage, not in logical order
		[[nodiscard]] constexpr std::array<T, N> const &array() const { return data; }
	};
}  // namespace common
