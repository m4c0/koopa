export module koopa:io;
import :str;
import :type_traits;
import jute;
import traits;

using namespace traits;

export namespace koopa {
  class input {
    jute::view m_data;

  public:
    explicit constexpr input(jute::view c) noexcept : m_data { c } {
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
      return m_data.size() > 0;
    }

    [[nodiscard]] constexpr bool operator==(const input & o) const noexcept {
      return m_data == o.m_data;
    }

    [[nodiscard]] constexpr char peek() const noexcept {
      return m_data[0];
    }
    [[nodiscard]] constexpr jute::view peek(size_t n) const noexcept {
      return m_data.subview(n).before;
    }
    [[nodiscard]] constexpr input take(size_t n) const noexcept {
      return input { m_data.subview(n).after }; // NOLINT
    }

    [[nodiscard]] constexpr size_t distance(const input o) const noexcept {
      if (m_data.size() == 0) return o.m_data.size();
      return m_data.data() - o.m_data.data();
    }
  };

  struct error {
    str message;
  };
  [[nodiscard]] inline constexpr auto operator==(const error & a, const error & b) noexcept {
    return a.message == b.message;
  }

  template<typename Tp>
  class output {
    Tp m_value {};
    error m_error {};
    input m_remainder;

  public:
    using type = Tp;

    constexpr output(error v, input r) noexcept : m_error(static_cast<struct error &&>(v)), m_remainder(r) {
    }
    constexpr output(Tp v, input r) noexcept : m_value(v), m_remainder(r) {
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept {
      return (*(m_error.message)).size() == 0;
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
      return output<T> { koopa::error { m_error.message + "\ncaused by: " + v }, m_remainder };
    }
    [[nodiscard]] constexpr output<Tp> with_error(jute::view msg) const noexcept {
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
  [[nodiscard]] inline constexpr output<Tp> fail(jute::view msg, const input in) noexcept {
    return output<Tp> { error { str { msg } }, in };
  }
  template<typename Tp>
  [[nodiscard]] inline constexpr output<Tp> fail(str && msg, const input in) noexcept {
    return output<Tp> { error { msg }, in };
  }

  inline constexpr input operator"" _i(const char * str, size_t len) noexcept {
    return input { jute::view { str, len } };
  }
}
