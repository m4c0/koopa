#pragma once

#include "koopa/io.hpp"

namespace koopa {
  template<typename T>
  static constexpr auto constant(T && t) noexcept {
    return [t](input in) noexcept {
      return output<T> { t, in };
    };
  }

  template<typename T>
  static constexpr auto failure(std::string_view sv) noexcept {
    return [sv](input in) noexcept {
      return fail<T>(sv, in);
    };
  }

  static constexpr auto eof(input in) noexcept {
    return in ? fail<nullptr_t>("expecting eof", in) : output<nullptr_t> { nullptr, in };
  }
}
