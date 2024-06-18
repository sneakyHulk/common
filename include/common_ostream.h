#pragma once

#include <array>
#include <list>
#include <map>
#include <ostream>
#include <tuple>
#include <unordered_map>
#include <vector>

// make std::array printable
template <typename T, std::size_t SIZE>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::array<T, SIZE> const& arr) {
	stream << '(';

#if __cplusplus < 202002L
	char space[]{0, 0, 0};
	for (auto const& e : arr) {
#else
	for (char space[]{0, 0, 0}; auto const& e : arr) {
#endif
		stream << space << e, *space = ',', *(space + 1) = ' ';
	}

	stream << ')';

	return stream;
}
template <typename T, std::size_t SIZE>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::array<T, SIZE> const&& arr) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(arr)>(arr));
}

// make std::tuple printable
template <typename... T>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::tuple<T...> const& tup) {
	static auto print_tuple = []<std::size_t... I>(std::ostream& stream, std::tuple<T...> const& tup, std::index_sequence<I...>) {
		char space[]{0, 0, 0};
		(... << (stream << space << std::get<I>(tup), *space = ',', *(space + 1) = ' '));
	};

	stream << '(';
	print_tuple(stream, tup, std::make_index_sequence<sizeof...(T)>());
	stream << ')';

	return stream;
}
template <typename... T>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::tuple<T...> const&& tup) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(tup)>(tup));
}

// make std::pair printable
template <typename T1, typename T2>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::pair<T1, T2> const& pair) {
	stream << '(';
	stream << pair.first << ", " << pair.second;
	stream << ')';

	return stream;
}
template <typename T1, typename T2>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::pair<T1, T2> const&& pair) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(pair)>(pair));
}

// make std::vector printable
template <typename T, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::vector<T, Alloc> const& vec) {
	stream << '[';

#if __cplusplus < 202002L
	char space[]{0, 0, 0};
	for (auto const& e : vec) {
#else
	for (char space[]{0, 0, 0}; auto const& e : vec) {
#endif
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

#if __cplusplus < 202002L
	char space[]{0, 0, 0, 0, 0};
	for (auto const& e : list) {
#else
	for (char space[]{0, 0, 0, 0, 0}; auto const& e : list) {
#endif
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

#if __cplusplus < 202002L
	char space[]{0, 0, 0};
	for (auto const& [key, value] : map) {
#else
	for (char space[]{0, 0, 0}; auto const& [key, value] : map) {
#endif
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

#if __cplusplus < 202002L
	char space[]{0, 0, 0};
	for (auto const& [key, value] : map) {
#else
	for (char space[]{0, 0, 0}; auto const& [key, value] : map) {
#endif
		stream << space << '\'' << key << "': '" << value << '\'', *space = ',', *(space + 1) = ' ';
	}

	stream << "}";

	return stream;
}
template <typename Key, typename T, typename Hash, typename KeyEqual, typename Alloc>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::unordered_map<Key, T, Hash, KeyEqual, Alloc> const&& vec) {
	return operator<<(std::forward<decltype(stream)>(stream), std::forward<decltype(vec)>(vec));
}