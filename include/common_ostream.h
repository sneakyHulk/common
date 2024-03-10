#pragma once

#include <array>
#include <list>
#include <map>
#include <ostream>
#include <unordered_map>
#include <vector>

// make std::array printable
template <typename T, std::size_t SIZE>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::array<T, SIZE> const& arr) {
	stream << '(';
	for (char space[]{0, 0, 0}; auto const& e : arr) {
		stream << space << e, *space = ',', *(space + 1) = ' ';
	}
	stream << ')';

	return stream;
}
template <typename T, std::size_t SIZE>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::array<T, SIZE> const&& arr) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(arr)>(arr));
}

// make std::vector printable
template <typename T, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::vector<T, Alloc> const& vec) {
	stream << '[';
	for (char space[]{0, 0, 0}; auto const& e : vec) {
		stream << space << e, *space = ',', *(space + 1) = ' ';
	}
	stream << ']';

	return stream;
}
template <typename T, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::vector<T, Alloc> const&& vec) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(vec)>(vec));
}

// make std::list printable
template <typename T, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::list<T, Alloc> const& list) {
	stream << "- ";
	for (char space[]{0, 0, 0, 0, 0}; auto const& e : list) {
		stream << space << e, *space = ' ', *(space + 1) = '-', *(space + 2) = '>', *(space + 3) = ' ';
	}
	stream << " -";

	return stream;
}
template <typename T, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::list<T, Alloc> const&& vec) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(vec)>(vec));
}

// make std::map printable
template <typename Key, typename T, typename Comp, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::map<Key, T, Comp, Alloc> const& map) {
	stream << "{";
	for (char space[]{0, 0, 0}; auto const& [key, value] : map) {
		stream << space << '\'' << key << "': '" << value << '\'', *space = ',', *(space + 1) = ' ';
	}
	stream << "}";

	return stream;
}
template <typename Key, typename T, typename Comp, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::map<Key, T, Comp, Alloc> const&& vec) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(vec)>(vec));
}

// make std::unordered_map printable
template <typename Key, typename T, typename Hash, typename KeyEqual, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::unordered_map<Key, T, Hash, KeyEqual, Alloc> const& map) {
	stream << "{";
	for (char space[]{0, 0, 0}; auto const& [key, value] : map) {
		stream << space << '\'' << key << "': '" << value << '\'', *space = ',', *(space + 1) = ' ';
	}
	stream << "}";

	return stream;
}
template <typename Key, typename T, typename Hash, typename KeyEqual, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::unordered_map<Key, T, Hash, KeyEqual, Alloc> const&& vec) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(vec)>(vec));
}