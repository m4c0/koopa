#pragma once

#include "koopa/str.hpp"
#include "koopa/type_traits.hpp"

#include <string_view>

namespace koopa {
  class input {
    std::string_view m_chars;

  public:
    explicit constexpr input(std::string_view c) noexcept : m_chars(c) {
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
      return !m_chars.empty();
    }

    [[nodiscard]] constexpr bool operator==(const input & o) const noexcept {
      return m_chars == o.m_chars;
    }

    [[nodiscard]] constexpr char peek() const noexcept {
      return m_chars[0];
    }
    [[nodiscard]] constexpr std::string_view peek(size_t n) const noexcept {
      return m_chars.substr(0, n);
    }
    [[nodiscard]] constexpr input take(size_t n) const noexcept {
      return input { m_chars.substr(n) };
    }

    [[nodiscard]] constexpr size_t distance(const input o) const noexcept {
      return o.m_chars.data() - m_chars.data();
    }
  };

  struct error {
    str message;
  };
  [[nodiscard]] static constexpr auto operator==(const error & a, const error & b) noexcept {
    return a.message == b.message;
  }

  template<typename Tp>
  class output {
    Tp m_value {};
    error m_error {};
    input m_remainder;

  public:
    using type = Tp;

    constexpr output(error v, input r) noexcept : m_error(std::move(v)), m_remainder(r) {
    }
    constexpr output(Tp v, input r) noexcept : m_value(v), m_remainder(r) {
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
      return (*(m_error.message)).empty();
    }
    [[nodiscard]] constexpr const Tp & operator*() const noexcept {
      return m_value;
    }

    [[nodiscard]] constexpr bool operator==(const output<Tp> & o) const noexcept {
      return m_value == o.m_value && m_remainder == o.m_remainder && m_error == o.m_error;
    }

    [[nodiscard]] constexpr const auto & error() const noexcept {
      return m_error;
    }
    [[nodiscard]] constexpr input remainder() const noexcept {
      return m_remainder;
    }

    template<typename T = Tp>
    [[nodiscard]] constexpr output<T> with_cause(auto v) const noexcept {
      using namespace std::string_view_literals;
      return output<T> { koopa::error { m_error.message + "\ncaused by: " + v }, m_remainder };
    }
    [[nodiscard]] constexpr output<Tp> with_error(std::string_view msg) const noexcept {
      return output<Tp> { koopa::error { str { msg } }, m_remainder };
    }
    template<typename T>
    [[nodiscard]] constexpr output<T> with_error_type() const noexcept {
      return output<T> { m_error, m_remainder };
    }
    template<typename T>
    [[nodiscard]] constexpr output<T> with_value(T v) const noexcept {
      return output<T> { v, m_remainder };
    }
  };

  template<not_same_as<error> Tp>
  output(Tp, input) -> output<Tp>;

  template<typename Tp>
  [[nodiscard]] static constexpr output<Tp> fail(std::string_view msg, const input in) noexcept {
    return output<Tp> { error { str { msg } }, in };
  }

  static constexpr input operator"" _i(const char * str, size_t len) noexcept {
    return input { std::string_view { str, len } };
  }
}
