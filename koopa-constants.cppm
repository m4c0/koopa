export module koopa:constants;
import :io;
import jute;

export namespace koopa {
  template<typename T>
  inline constexpr auto constant(T && t) noexcept {
    return [t](input in) noexcept {
      return output<T> { t, in };
    };
  }

  template<typename T>
  inline constexpr auto failure(jute::view sv) noexcept {
    return [sv](input in) noexcept {
      return fail<T>(sv, in);
    };
  }

  inline constexpr auto eof(input in) noexcept {
    using nullptr_t = decltype(nullptr);
    return in ? fail<nullptr_t>("expecting eof", in) : output<nullptr_t> { nullptr, in };
  }
}
