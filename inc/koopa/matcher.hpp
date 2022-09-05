#pragma once

#include "koopa/io.hpp"

namespace koopa {
  static constexpr auto any() noexcept {
    return [](const input in) noexcept {
      if (!in) return fail<char>("eof while waiting for any data", in);
      return output<char> { in.peek(), in.take(1) };
    };
  }

  static constexpr auto match(char chr) noexcept {
    return [chr](const input in) noexcept {
      if (!in) return fail<char>("eof while waiting for char", in);
      if (in.peek() != chr) return fail<char>("mismatched char", in);
      return output<char> { chr, in.take(1) };
    };
  }

  static constexpr auto match(std::string_view sv) noexcept {
    return [sv](const input in) noexcept {
      const auto len = sv.length();
      if (in.peek(len) != sv) return fail<std::string_view>("mismatched string", in);
      return output<std::string_view> { sv, in.take(sv.length()) };
    };
  }
}
