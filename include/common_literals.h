#pragma once

#include <cstdint>

[[maybe_unused]] inline constexpr char operator""_ch(unsigned long long int const num) { return num; }
[[maybe_unused]] inline constexpr std::uint8_t operator""_u8(unsigned long long int const num) { return static_cast<std::uint8_t>(num); }
[[maybe_unused]] constexpr std::uint16_t operator""_u16(unsigned long long int const num) { return static_cast<std::uint16_t>(num); }
[[maybe_unused]] constexpr std::int16_t operator""_i16(unsigned long long int const num) { return static_cast<std::int16_t>(num); }
