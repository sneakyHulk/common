#pragma once

#include <cstdint>

[[maybe_unused]] inline constexpr char operator""_ch(unsigned long long int num) { return num; }
[[maybe_unused]] inline constexpr std::uint8_t operator""_u8(unsigned long long int num) { return  static_cast<std::uint8_t>(num); }
