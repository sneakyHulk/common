#include <common_parser.h>

#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <random>
#include <span>
#include <string>
#include <string_view>
#include <vector>

using namespace common;

// CRC-16/ARC, same as boost::crc_16_type
class Crc16 {
	std::uint16_t value = 0;

   public:
	constexpr void process_byte(std::uint8_t const byte) {
		value ^= byte;
		for (int k = 0; k < 8; ++k) value = (value & 1) ? (value >> 1) ^ 0xA001 : value >> 1;
	}
	[[nodiscard]] constexpr std::uint16_t checksum() const { return value; }
};

// CRC-8, polynomial 0x07, same as boost::crc_optimal<8, 0x07, 0x00, 0x00, false, false>
class Crc8 {
	std::uint8_t value = 0;

   public:
	constexpr void process_byte(std::uint8_t const byte) {
		value ^= byte;
		for (int k = 0; k < 8; ++k) value = static_cast<std::uint8_t>((value & 0x80) ? (value << 1) ^ 0x07 : value << 1);
	}
	[[nodiscard]] constexpr std::uint8_t checksum() const { return value; }
};

// standard check values for "123456789"
template <typename Crc>
constexpr auto check_value() {
	Crc crc;
	for (char const c : std::string_view{"123456789"}) crc.process_byte(static_cast<std::uint8_t>(c));
	return crc.checksum();
}
static_assert(check_value<Crc16>() == 0xBB3D);
static_assert(check_value<Crc8>() == 0xF4);

// local copies of the wire messages, so common does not depend on permatracks_msg
#pragma pack(push, 1)
struct Header {
	std::uint64_t timestamp;
};

struct TimeSyncRequestWireMessage : Header {
	static constexpr char tag = 'R';
};

struct TimeSyncResponseWireMessage : Header {
	static constexpr char tag = 'R';
	std::uint64_t t1;
};

struct MagneticFluxDensityDataRaw {
	std::array<std::uint8_t, 7> bytes;
};

template <std::size_t N, typename T>
struct MagneticFluxDensityRawWireMessage : Header {
	static constexpr char tag = 'M';
	std::int32_t scale;
	std::array<T, N> data;
};

template <std::size_t N>
struct TemperatureDataRawWireMessage : Header {
	static constexpr char tag = 'T';
	float offset;
	float scale;
	std::array<std::uint16_t, N> data;
};

struct AccelerationWireMessage : Header {
	static constexpr char tag = 'A';
	float scale;
	std::int16_t ax, ay, az;
};

struct GyroWireMessage : Header {
	static constexpr char tag = 'G';
	float scale;
	std::int16_t gx, gy, gz;
};

struct QuaternionWireMessage : Header {
	static constexpr char tag = 'Q';
	std::array<float, 4> data;
};

struct GravityWireMessage : Header {
	static constexpr char tag = 'V';
	std::array<float, 3> data;
};

struct GyroBiasWireMessage : Header {
	static constexpr char tag = 'B';
	std::array<float, 3> data;
};
#pragma pack(pop)

struct InfoWireMessage : Header {
	static constexpr char tag = 'I';
	static constexpr std::size_t max_payload_size = 255;
	std::string payload;
};

using Mag = MagneticFluxDensityRawWireMessage<10, MagneticFluxDensityDataRaw>;
using Temperature = TemperatureDataRawWireMessage<4>;
using TestParser = Parser<Header, Crc16, TimeSyncRequestWireMessage, Mag, Temperature, AccelerationWireMessage, GyroWireMessage, QuaternionWireMessage, GravityWireMessage, GyroBiasWireMessage, InfoWireMessage>;

// content = header + fields (fixed) or header + payload (variable, then length = payload size)
std::vector<std::uint8_t> make_frame(char tag, std::vector<std::uint8_t> const &content, std::optional<std::uint8_t> length = std::nullopt) {
	std::vector<std::uint8_t> out{static_cast<std::uint8_t>(tag)};
	out.insert(out.end(), content.begin(), content.end());
	if (length) out.push_back(*length);

	Crc16 crc;
	for (auto const byte : content) crc.process_byte(byte);
	out.push_back(static_cast<std::uint8_t>(crc.checksum() & 0xFF));
	out.push_back(static_cast<std::uint8_t>((crc.checksum() >> 8) & 0xFF));

	out.push_back(static_cast<std::uint8_t>(tag));
	return out;
}

template <typename T>
std::vector<std::uint8_t> to_bytes(T const &value) {
	std::vector<std::uint8_t> out;
	for (auto const byte : std::as_bytes(std::span{&value, 1})) out.push_back(static_cast<std::uint8_t>(byte));
	return out;
}

template <Encodable T>
std::vector<std::uint8_t> make_frame(T const &message) {
	return make_frame(T::tag, to_bytes(message));
}

std::vector<TestParser::Result> parse_all(TestParser &parser, std::vector<std::uint8_t> const &bytes) {
	std::vector<TestParser::Result> out;
	for (auto r = parser.parse(bytes); r; r = parser.parse()) out.push_back(std::move(*r));
	return out;
}

AccelerationWireMessage accel(std::int16_t ax, std::uint64_t timestamp) {
	AccelerationWireMessage m{};
	m.scale = 1.f;
	m.ax = ax;
	m.timestamp = timestamp;
	return m;
}

void test_accel() {
	TestParser parser;
	auto const results = parse_all(parser, make_frame(accel(1000, 42)));
	assert(results.size() == 1);
	auto const &a = std::get<AccelerationWireMessage>(results[0]);
	assert(a.ax == 1000 && a.scale == 1.f && a.timestamp == 42);
}

void test_split_byte_by_byte_with_garbage() {
	std::vector<std::uint8_t> stream{0x00, 'A', 0x13, 'G'};
	auto const a = make_frame(accel(1, 1));
	stream.insert(stream.end(), a.begin(), a.end());
	stream.insert(stream.end(), {0x01, 'G', 0x02});
	stream.insert(stream.end(), a.begin(), a.end());

	TestParser parser;
	std::size_t n = 0;
	for (auto const byte : stream) n += parse_all(parser, {byte}).size();
	assert(n == 2);
}

void test_info_and_time_sync() {
	std::string const text = "hello";
	std::uint64_t const info_timestamp = 11;
	auto info = to_bytes(info_timestamp);
	info.insert(info.end(), text.begin(), text.end());

	auto stream = make_frame('I', info, static_cast<std::uint8_t>(text.size()));
	TimeSyncRequestWireMessage request{};
	request.timestamp = 22;
	auto const t = make_frame(request);
	stream.insert(stream.end(), t.begin(), t.end());

	TestParser parser;
	auto const results = parse_all(parser, stream);
	// yielded in the order of the parser's message types
	assert(results.size() == 2);
	assert(std::get<TimeSyncRequestWireMessage>(results[0]).timestamp == 22);
	assert(std::get<InfoWireMessage>(results[1]).payload == text);
	assert(std::get<InfoWireMessage>(results[1]).timestamp == info_timestamp);
}

// a new message is just a struct
#pragma pack(push, 1)
struct TemperatureMessage : Header {
	static constexpr char tag = 'K';
	std::int16_t centi_celsius;
	std::uint8_t sensor;
};
#pragma pack(pop)

// also checks the order: TemperatureMessage comes first although it is second in the stream,
// and the accel scan does not overwrite it (short-circuit in scan_first)
void test_custom_message_and_order() {
	Parser<Header, Crc16, TemperatureMessage, AccelerationWireMessage> parser;
	TemperatureMessage temperature{};
	temperature.timestamp = 33;
	temperature.centi_celsius = 2150;
	temperature.sensor = 3;

	auto stream = make_frame(accel(5, 5));
	auto const k = make_frame(temperature);
	stream.insert(stream.end(), k.begin(), k.end());

	std::vector<decltype(parser)::Result> results;
	for (auto r = parser.parse(stream); r; r = parser.parse()) results.push_back(*r);
	assert(results.size() == 2);
	assert(std::get<TemperatureMessage>(results[0]).centi_celsius == 2150);
	assert(std::get<TemperatureMessage>(results[0]).timestamp == 33);
	assert(std::get<AccelerationWireMessage>(results[1]).ax == 5);
}

void test_buffer_keeps_only_unparseable_tail() {
	TestParser parser;
	std::vector<std::uint8_t> garbage(1000, 0x00);
	assert(parse_all(parser, garbage).empty());

	// and still finds a frame that arrives afterwards
	assert(parse_all(parser, make_frame(accel(3, 3))).size() == 1);
}

void test_time_sync_round_trip() {
	TimeSyncResponseWireMessage response{};
	response.timestamp = 222;
	response.t1 = 111;

	auto const frame = encode<Crc16>(response);
	assert(std::vector<std::uint8_t>(frame.begin(), frame.end()) == make_frame(response));

	Parser<Header, Crc16, TimeSyncResponseWireMessage> parser;
	auto const result = parser.parse(frame);
	assert(result && std::get<TimeSyncResponseWireMessage>(*result).t1 == 111 && std::get<TimeSyncResponseWireMessage>(*result).timestamp == 222);
}

template <Encodable M>
void append_frame(std::vector<std::uint8_t> &stream, std::uint64_t const timestamp) {
	M message{};
	message.timestamp = timestamp;
	auto const frame = encode<Crc16>(message);
	stream.insert(stream.end(), frame.begin(), frame.end());
}

// n frames of every message type in TestParser, interleaved, timestamp = running number per type
std::vector<std::uint8_t> all_messages_stream(std::uint64_t const n) {
	std::vector<std::uint8_t> stream;
	for (std::uint64_t i = 0; i < n; ++i) {
		append_frame<TimeSyncRequestWireMessage>(stream, i);
		append_frame<Mag>(stream, i);
		append_frame<Temperature>(stream, i);
		append_frame<AccelerationWireMessage>(stream, i);
		append_frame<GyroWireMessage>(stream, i);
		append_frame<QuaternionWireMessage>(stream, i);
		append_frame<GravityWireMessage>(stream, i);
		append_frame<GyroBiasWireMessage>(stream, i);

		std::string const text(i % 256, static_cast<char>('a' + i % 26));  // 0 .. 255 bytes
		auto info = to_bytes(i);
		info.insert(info.end(), text.begin(), text.end());
		auto const frame = make_frame('I', info, static_cast<std::uint8_t>(text.size()));
		stream.insert(stream.end(), frame.begin(), frame.end());
	}
	return stream;
}

// every type exactly n times, in stream order per type
void check_all_messages(std::vector<TestParser::Result> const &results, std::uint64_t const n) {
	std::array<std::uint64_t, std::variant_size_v<TestParser::Result>> next{};
	for (auto const &result : results) {
		auto &expected = next[result.index()];
		std::visit([&](auto const &message) { assert(message.timestamp == expected); }, result);
		if (auto const *info = std::get_if<InfoWireMessage>(&result)) assert(info->payload.size() == expected % 256);
		++expected;
	}
	for (auto const count : next) assert(count == n);
}

void test_all_messages_random_chunks() {
	auto const stream = all_messages_stream(300);
	std::mt19937 rng(1);

	TestParser parser;
	std::vector<TestParser::Result> results;
	for (std::size_t pos = 0; pos < stream.size();) {
		// up to the longest frame per call, the remaining half of the buffer holds the unparsed tail
		auto const n = std::min<std::size_t>(1 + rng() % (TestParser::capacity / 2), stream.size() - pos);
		for (auto r = parser.parse(std::span{stream}.subspan(pos, n)); r; r = parser.parse()) results.push_back(*r);
		pos += n;
	}
	check_all_messages(results, 300);
}

// like boost::asio read_some: read directly into the parser, short reads included
void test_all_messages_via_writable() {
	auto const stream = all_messages_stream(300);
	std::mt19937 rng(2);

	TestParser parser;
	std::vector<TestParser::Result> results;
	for (std::size_t pos = 0; pos < stream.size();) {
		auto const free = parser.writable();
		assert(!free.empty());

		auto const n = std::min<std::size_t>({1 + rng() % free.size(), free.size(), stream.size() - pos});
		std::copy_n(stream.begin() + static_cast<std::ptrdiff_t>(pos), n, free.begin());
		parser.commit(n);
		pos += n;

		while (auto r = parser.parse()) results.push_back(*r);
	}
	check_all_messages(results, 300);
}

void test_overflow_drops_oldest_and_recovers() {
	TestParser parser;
	std::vector<std::uint8_t> garbage(10 * TestParser::capacity, 'I');  // worst case: every byte could start an info frame
	assert(parse_all(parser, garbage).empty());
	assert(parse_all(parser, make_frame(accel(3, 3))).size() == 1);
}

// writes bytes into the parser the way read_some would, as many as writable() allows per call
template <typename Parser>
void feed_via_writable(Parser &parser, std::vector<std::uint8_t> const &bytes, std::vector<typename Parser::Result> &results) {
	for (std::size_t pos = 0; pos < bytes.size();) {
		auto const free = parser.writable();
		assert(!free.empty());

		auto const n = std::min(free.size(), bytes.size() - pos);
		std::copy_n(bytes.begin() + static_cast<std::ptrdiff_t>(pos), n, free.begin());
		parser.commit(n);
		pos += n;

		while (auto r = parser.parse()) results.push_back(*r);
	}
}

void test_writable_fresh_parser_is_whole_buffer() {
	TestParser parser;
	assert(parser.writable().size() == TestParser::capacity);
}

void test_commit_zero_is_noop() {
	TestParser parser;
	auto const before = parser.writable().size();
	parser.commit(0);
	assert(parser.writable().size() == before);
	assert(!parser.parse());
}

// a frame delivered in two short reads (commit less than writable().size())
void test_commit_partial_frame() {
	auto const frame = make_frame(accel(9, 9));
	TestParser parser;

	auto free = parser.writable();
	std::copy_n(frame.begin(), 5, free.begin());
	parser.commit(5);
	assert(!parser.parse());

	free = parser.writable();
	std::copy(frame.begin() + 5, frame.end(), free.begin());
	parser.commit(frame.size() - 5);

	auto const result = parser.parse();
	assert(result && std::get<AccelerationWireMessage>(*result).ax == 9);
	assert(!parser.parse());
}

// shifts the frame through every position of the ring buffer, so it also lies across the physical end
void test_frame_across_wrap_at_every_offset() {
	auto const frame = make_frame(accel(4, 44));

	for (std::size_t offset = 0; offset < TestParser::capacity; ++offset) {
		TestParser parser;
		std::vector<TestParser::Result> results;

		feed_via_writable(parser, std::vector<std::uint8_t>(offset, 0x00), results);  // garbage without tags
		feed_via_writable(parser, frame, results);

		assert(results.size() == 1);
		auto const &a = std::get<AccelerationWireMessage>(results[0]);
		assert(a.ax == 4 && a.timestamp == 44);
	}
}

// after parse() returned nullopt at least half of the buffer is free, possibly split in two parts at the wrap
void test_writable_after_drain_is_at_least_half() {
	auto const stream = all_messages_stream(50);
	std::mt19937 rng(3);

	TestParser parser;
	std::vector<TestParser::Result> results;
	for (std::size_t pos = 0; pos < stream.size();) {
		auto const n = std::min<std::size_t>(1 + rng() % (TestParser::capacity / 2), stream.size() - pos);
		std::vector<std::uint8_t> const chunk(stream.begin() + static_cast<std::ptrdiff_t>(pos), stream.begin() + static_cast<std::ptrdiff_t>(pos + n));
		feed_via_writable(parser, chunk, results);
		pos += n;

		// probe the free space on a copy, so the real parser is not disturbed
		auto probe = parser;
		auto const first = probe.writable();
		std::fill(first.begin(), first.end(), 0x00);
		probe.commit(first.size());
		auto const second = probe.writable();
		assert(first.size() + second.size() >= TestParser::capacity / 2);
	}
	check_all_messages(results, 50);
}

// without parse() the buffer runs full: writable() becomes empty, parse() frees it again and nothing is lost
void test_writable_empty_when_full_and_recovers() {
	auto const frame = make_frame(accel(1, 1));
	TestParser parser;

	std::size_t frames = 0;
	for (std::size_t pos = 0;;) {
		auto const free = parser.writable();
		if (free.empty()) break;
		auto const n = std::min(free.size(), frame.size() - pos);
		std::copy_n(frame.begin() + static_cast<std::ptrdiff_t>(pos), n, free.begin());
		parser.commit(n);
		pos += n;
		if (pos == frame.size()) {
			pos = 0;
			++frames;
		}
	}
	assert(parser.writable().empty());

	std::size_t parsed = 0;
	while (parser.parse()) ++parsed;
	assert(parsed == frames);
	assert(!parser.writable().empty());
}

// the checksum is a parameter: the same messages with crc8 instead of crc16
void test_crc8() {
	static_assert(checksum_size<Crc8> == 1);
	static_assert(checksum_size<Crc16> == 2);
	static_assert(!Checksum<int>);

	Parser<Header, Crc8, AccelerationWireMessage, InfoWireMessage> parser;
	static_assert(decltype(parser)::capacity == 2 * (1 + 8 + 255 + 1 + 1 + 1));

	auto const frame = encode<Crc8>(accel(12, 34));
	static_assert(std::tuple_size_v<std::remove_cvref_t<decltype(frame)>> == 1 + sizeof(AccelerationWireMessage) + 1 + 1);

	auto const result = parser.parse(frame);
	assert(result && std::get<AccelerationWireMessage>(*result).ax == 12 && std::get<AccelerationWireMessage>(*result).timestamp == 34);

	// a crc16 frame is not accepted by a crc8 parser
	auto const wrong = encode<Crc16>(accel(1, 1));
	assert(!parser.parse(wrong));
}

int main() {
	test_crc8();
	test_writable_fresh_parser_is_whole_buffer();
	test_commit_zero_is_noop();
	test_commit_partial_frame();
	test_frame_across_wrap_at_every_offset();
	test_writable_after_drain_is_at_least_half();
	test_writable_empty_when_full_and_recovers();
	test_all_messages_random_chunks();
	test_all_messages_via_writable();
	test_overflow_drops_oldest_and_recovers();
	test_time_sync_round_trip();
	test_accel();
	test_split_byte_by_byte_with_garbage();
	test_info_and_time_sync();
	test_custom_message_and_order();
	test_buffer_keeps_only_unparseable_tail();
	std::cout << "all common_parser tests passed" << std::endl;
}
