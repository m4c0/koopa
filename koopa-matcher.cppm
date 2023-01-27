export module koopa:matcher;
import :io;
import :str;
import jute;

export namespace koopa {
  inline constexpr auto any() noexcept {
    return [](const input in) noexcept {
      if (!in) return fail<char>("eof while waiting for any data", in);
      return output<char> { in.peek(), in.take(1) };
    };
  }

  inline constexpr auto match(char chr) noexcept {
    return [chr](const input in) noexcept {
      if (!in) return fail<char>("eof while waiting for '"_ks + chr + "'", in);
      const auto got = in.peek();
      if (got != chr) return fail<char>("expecting '"_ks + chr + "' got '" + got + "'", in);
      return output<char> { chr, in.take(1) };
    };
  }

  inline constexpr auto match(jute::view sv) noexcept {
    return [sv](const input in) noexcept {
      const auto len = sv.size();
      const auto got = in.peek(len);
      if (got != sv) return fail<jute::view>("expecting '"_ks + sv + "' got '" + got + "'", in);
      return output<jute::view> { sv, in.take(sv.size()) };
    };
  }
}
