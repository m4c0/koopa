#pragma once

#include "koopa/io.hpp"

namespace koopa {
  [[nodiscard]] inline constexpr auto take_u8() noexcept {
    return [](input in) noexcept -> output<uint8_t> {
      if (!in) return fail<uint8_t>("eof while expecting u8", in);
      return { static_cast<uint8_t>(in.peek()), in.take(1) };
    };
  }
  [[nodiscard]] inline constexpr auto take_u16le() noexcept {
    return [](input in) noexcept -> output<uint16_t> {
      const auto sv = in.peek(2);
      if (sv.size() != 2) return fail<uint16_t>("eof while expecting u16le", in);
      const auto a = static_cast<uint16_t>(static_cast<uint8_t>(sv[0]));
      const auto b = static_cast<uint16_t>(static_cast<uint8_t>(sv[1]) << 8U);
      return { static_cast<uint16_t>(b | a), in.take(2) };
    };
  }
  [[nodiscard]] inline constexpr auto take_u32le() noexcept {
    return [](input in) noexcept -> output<uint32_t> {
      const auto sv = in.peek(4);
      if (sv.size() != 4) return fail<uint32_t>("eof while expecting u32le", in);
      const auto a = static_cast<uint32_t>(static_cast<uint8_t>(sv[0]));
      const auto b = static_cast<uint32_t>(static_cast<uint8_t>(sv[1]) << 8U);
      const auto c = static_cast<uint32_t>(static_cast<uint8_t>(sv[2]) << 16U);
      const auto d = static_cast<uint32_t>(static_cast<uint8_t>(sv[3]) << 24U);
      return { d | c | b | a, in.take(4) };
    };
  }

  [[nodiscard]] inline constexpr auto match_u8(uint8_t exp) noexcept {
    return [exp, m = take_u8()](input in) noexcept {
      auto got = m(in);
      return (!got || *got == exp) ? got : fail<uint8_t>("mismatched u8", in);
    };
  }

  [[nodiscard]] inline constexpr auto match_u16le(uint16_t exp) noexcept {
    return [exp, m = take_u16le()](input in) noexcept {
      auto got = m(in);
      return (!got || *got == exp) ? got : fail<uint16_t>("mismatched u16le", in);
    };
  }

  [[nodiscard]] inline constexpr auto match_u32le(uint32_t exp) noexcept {
    return [exp, m = take_u32le()](input in) noexcept {
      auto got = m(in);
      return (!got || *got == exp) ? got : fail<uint32_t>("mismatched u32le", in);
    };
  }
}
