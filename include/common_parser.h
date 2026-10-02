#pragma once

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <type_traits>
#include <utility>
#include <variant>

#include "common_ring_buffer.h"

namespace common {

	// checksum over everything between the tags except length and checksum, stored little endian, e.g. boost::crc_16_type
	template <typename C>
	concept Checksum = std::default_initializable<C> && requires(C crc, C const const_crc, std::uint8_t byte) {
		crc.process_byte(byte);
		{ const_crc.checksum() } -> std::unsigned_integral;
	};

	// bytes of the checksum on the wire, the size of the type checksum() returns
	template <Checksum Crc>
	inline constexpr std::size_t checksum_size = sizeof(decltype(std::declval<Crc const &>().checksum()));

	// A message is a plain struct with static constexpr char tag that derives from a Header (e.g. a timestamp), Header is a parameter of the Parser;
	// fixed size:    trivially copyable and packed, the struct is the payload        -> [tag][header][fields][crc16][tag]
	// variable size: static constexpr std::size_t max_payload_size + member payload  -> [tag][header][payload: n][n][crc][tag]
	template <typename T>
	concept Tagged = requires {
		{ T::tag } -> std::convertible_to<char>;
	};

	template <typename T, typename Header>
	concept VariableMessage = Tagged<T> && std::derived_from<T, Header> && std::is_trivially_copyable_v<Header> && std::default_initializable<T> && requires(T t, std::uint8_t byte) {
		{ T::max_payload_size } -> std::convertible_to<std::size_t>;
		t.payload.push_back(byte);
	};

	template <typename T, typename Header>
	concept FixedMessage = Tagged<T> && std::derived_from<T, Header> && !VariableMessage<T, Header> && std::is_trivially_copyable_v<T>;

	template <typename T, typename Header>
	concept WireMessage = FixedMessage<T, Header> || VariableMessage<T, Header>;

	// a fixed size message on the wire, the header is part of the struct
	template <typename M>
	concept Encodable = Tagged<M> && std::is_trivially_copyable_v<M>;

	template <Encodable M, Checksum Crc>
	inline constexpr std::size_t frame_size = 1 + sizeof(M) + checksum_size<Crc> + 1;

	template <typename Header, Checksum Crc, WireMessage<Header> M>
	consteval std::size_t max_frame_size() {
		if constexpr (FixedMessage<M, Header>)
			return frame_size<M, Crc>;
		else
			return 1 + sizeof(Header) + M::max_payload_size + 1 + checksum_size<Crc> + 1;
	}

	// [tag][header][fields][crc][tag], every byte assigned individually
	template <Checksum Crc, Encodable M>
	std::array<std::uint8_t, frame_size<M, Crc>> encode(M const &message) {
		std::array<std::uint8_t, frame_size<M, Crc>> out{};
		Crc crc;

		std::size_t i = 0;
		out[i++] = static_cast<std::uint8_t>(M::tag);
		for (auto const byte : std::as_bytes(std::span{&message, 1})) {
			out[i] = static_cast<std::uint8_t>(byte);
			crc.process_byte(out[i++]);
		}
		for (std::size_t k = 0; k < checksum_size<Crc>; ++k) out[i++] = static_cast<std::uint8_t>((crc.checksum() >> (8 * k)) & 0xFF);
		out[i] = static_cast<std::uint8_t>(M::tag);
		return out;
	}

	// Every message type scans the buffer with its own index, in the order of Ms.
	// Bytes are only erased once no message type can use them anymore.
	// The buffer holds twice the longest frame: after parse() returned nullopt at most one incomplete frame is left,
	// so there is always room for at least one more complete frame.
	template <typename Header, Checksum Crc, WireMessage<Header>... Ms>
	class Parser {
	   public:
		using Result = std::variant<Ms...>;
		static constexpr std::size_t capacity = 2 * std::max({max_frame_size<Header, Crc, Ms>()...});

	   private:
		ring_buffer<std::uint8_t, capacity> buffer;
		std::array<std::size_t, sizeof...(Ms)> indices{};

		// erases the bytes no M can use anymore
		void erase_unused() {
			auto const remove = std::ranges::min(indices);
			buffer.pop(remove);
			for (auto &index : indices) index -= remove;
		}

		[[nodiscard]] bool crc_matches(std::size_t const begin, std::size_t const size, std::size_t const crc_begin) const {
			Crc crc;
			for (std::size_t j = begin; j < begin + size; ++j) crc.process_byte(buffer[j]);
			auto const checksum = crc.checksum();
			for (std::size_t k = 0; k < checksum_size<Crc>; ++k) {
				if (buffer[crc_begin + k] != static_cast<std::uint8_t>((checksum >> (8 * k)) & 0xFF)) return false;
			}
			return true;
		}

		// assigns every byte of out, starting at buffer[begin]
		template <typename T>
		requires std::is_trivially_copyable_v<T> void read(std::size_t begin, T &out) const {
			for (auto &byte : std::as_writable_bytes(std::span{&out, 1})) byte = static_cast<std::byte>(buffer[begin++]);
		}

		// [tag][header][fields][crc][tag], the struct bytes include the header
		template <FixedMessage<Header> M>
		std::optional<Result> scan(std::size_t &index) {
			static constexpr std::size_t size = frame_size<M, Crc>;
			static constexpr auto tag = static_cast<std::uint8_t>(M::tag);

			while (index + size <= buffer.size()) {
				if (buffer[index] == tag && buffer[index + size - 1] == tag && crc_matches(index + 1, sizeof(M), index + 1 + sizeof(M))) {
					M message;
					read(index + 1, message);
					index += size;
					return message;
				}
				++index;
			}
			return std::nullopt;
		}

		// [tag][header][payload: n][n][crc][tag], the length sits at the end -> search for the end tag
		template <VariableMessage<Header> M>
		std::optional<Result> scan(std::size_t &index) {
			static constexpr std::size_t min_size = 1 + sizeof(Header) + 0 + 1 + checksum_size<Crc> + 1;
			static constexpr std::size_t max_size = min_size + M::max_payload_size;
			static constexpr auto tag = static_cast<std::uint8_t>(M::tag);
			static_assert(M::max_payload_size <= 255);

			for (std::size_t end = index + min_size - 1; end < buffer.size(); ++end) {
				if (buffer[end] != tag) continue;

				std::size_t const crc_begin = end - checksum_size<Crc>, payload_size = buffer[crc_begin - 1], size = min_size + payload_size;
				if (end + 1 < index + size) continue;  // would start before index

				if (std::size_t const begin = end + 1 - size; buffer[begin] == tag && crc_matches(begin + 1, sizeof(Header) + payload_size, crc_begin)) {
					M message;
					read(begin + 1, static_cast<Header &>(message));
					for (std::size_t k = begin + 1 + sizeof(Header); k < begin + 1 + sizeof(Header) + payload_size; ++k) message.payload.push_back(buffer[k]);
					index = end + 1;
					return message;
				}
			}

			// a frame that is not complete yet starts at the earliest max_size - 1 bytes before the end
			if (buffer.size() + 1 > max_size) index = std::max(index, buffer.size() + 1 - max_size);
			return std::nullopt;
		}

	   public:
		// zero copy input, e.g. boost::asio read_some or DMA:
		//   auto const free = parser.writable();
		//   parser.commit(port.read_some(boost::asio::buffer(free.data(), free.size())));
		//   while (auto const result = parser.parse()) ...
		// can be shorter than the free space when it wraps around the end of the ring buffer
		std::span<std::uint8_t> writable() {
			erase_unused();
			return buffer.writable();
		}

		void commit(std::size_t const n) { buffer.commit(n); }

		// tries scan<M> for every M in order and stops at the first one that finds a message (|| short-circuits left to right)
		// erases the bytes no M can use anymore once there is no message left
		// bytes that do not fit drop the oldest bytes, so call parse() until nullopt before passing new bytes
		std::optional<Result> parse(std::span<std::uint8_t const> bytes = {}) {
			for (auto const byte : bytes) {
				if (buffer.full()) erase_unused();
				if (buffer.full()) {  // overflow: drop the oldest byte
					buffer.pop();
					for (auto &index : indices) index -= index > 0;
				}
				buffer.push_back(byte);
			}

			return [this]<std::size_t... I>(std::index_sequence<I...>) {
				std::optional<Result> out;
				(... || (out = scan<Ms>(indices[I])).has_value());

				if (!out) erase_unused();
				return out;
			}(std::index_sequence_for<Ms...>{});
		}
	};

}  // namespace common
