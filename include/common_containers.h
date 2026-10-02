#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <queue>
#include <utility>
#include <vector>

namespace common {
	// Hash std::arrays for std::unordered_map -> std::unordered_map<std::array<..., ...>, ..., ArrayHasher>
	struct [[maybe_unused]] ArrayHasher {
		template <typename TYPE, std::size_t SIZE>
		std::size_t operator()(std::array<TYPE, SIZE> const &arr) const {
			std::size_t h = 0;
			for (TYPE const e : arr) {
				h ^= std::hash<TYPE>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
			}
			return h;
		}
	};

	// use this to store key-value pairs of any kind and to access only the highest key.
#if __cplusplus >= 202002L
	template <typename Key, typename Value>
	class [[maybe_unused]] pair_priority_queue
	    : public std::priority_queue<std::pair<Key, Value>, std::vector<std::pair<Key, Value>>, decltype([](std::pair<Key, Value> const &a, std::pair<Key, Value> const &b) { return a.first < b.first; })> {};
#endif
}  // namespace common
