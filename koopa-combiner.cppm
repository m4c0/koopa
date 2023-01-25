module;
#include <optional>
#include <string_view>

export module koopa:combiner;
import :io;
import :type_traits;
import traits;

export namespace koopa {
  template<parser PA, parser PB, typename AB>
  inline constexpr auto combine(PA && pa, PB && pb, AB && fn) noexcept {
    using T = call_result_t<AB, result_of<PA>, result_of<PB>>;

    return [pa, pb, fn](const input in) noexcept -> output<T> {
      const auto ra = pa(in);
      if (!ra) return ra.template with_error_type<T>();

      const auto rb = pb(ra.remainder());
      if (!rb) return rb.template with_error_type<T>();

      return rb.with_value(fn(*ra, *rb));
    };
  }

  template<parser PA, parser PB>
  inline constexpr auto operator,(PA && pa, PB && pb) noexcept {
    using T = result_of<PB>;

    return [pa, pb](const input in) noexcept -> output<T> {
      const auto ra = pa(in);
      if (!ra) return ra.template with_error_type<T>();

      const auto rb = pb(ra.remainder());
      if (!rb) return rb.template with_error_type<T>();

      return rb;
    };
  }

  template<parser PA, parser PB>
  inline constexpr auto operator-(PA && pa, PB && pb) noexcept {
    using T = result_of<PA>;

    return [pa, pb](const input in) noexcept -> output<T> {
      const auto ra = pa(in);
      if (!ra) return ra.template with_error_type<T>();

      const auto rb = pb(ra.remainder());
      if (!rb) return rb.template with_error_type<T>();

      return rb.with_value(*ra);
    };
  }

  template<parser PA, parser PB>
  inline constexpr auto operator|(PA && pa, PB && pb) noexcept {
    return [pa, pb](input in) noexcept -> output<result_of<PA>> {
      const auto ra = pa(in);
      if (ra) return ra;
      const auto rb = pb(in);
      if (rb) return rb;
      return rb.with_error("expecting different alternative");
    };
  }

  template<parser P>
  inline constexpr auto maybe(P && p) noexcept {
    return [p](const input in) noexcept -> output<std::optional<result_of<P>>> {
      const auto r = p(in);
      const auto v = r ? std::optional { *r } : std::nullopt;
      return r.with_value(v);
    };
  }

  template<parser P, typename F>
  inline constexpr auto map(F && f, P && p) noexcept {
    return [f, p](input in) noexcept {
      return f(p(in));
    };
  }

  template<parser P, typename F>
  inline constexpr auto fmap(F && f, P && p) noexcept {
    using FT = mapper_traits<F, result_of<P>>;
    using T = typename FT::result_t;
    return [f, p](input in) noexcept -> output<T> {
      const auto r = p(in);
      if (!r) return r.template with_error_type<T>();
      return r.with_value(FT::invoke(f, *r));
    };
  }

  template<parser If, parser Then, parser Else>
  inline constexpr auto ifelse(If && i, Then && t, Else && e) noexcept {
    using T = result_of<Then>;
    return [i, t, e](input in) noexcept -> output<T> {
      const auto pi = i(in);
      return pi ? t(pi.remainder()) : e(in);
    };
  }

  template<parser P>
  inline constexpr auto until(P && p) noexcept {
    return [p](const input in) noexcept -> output<std::string_view> {
      input sin = in;
      int idx = 0;
      while (sin && !p(sin)) {
        idx++;
        sin = sin.take(1);
      }
      return output { in.peek(idx), sin };
    };
  }

  template<parser P>
  inline constexpr auto whilst(P && p) noexcept {
    return [p](const input in) noexcept -> output<std::string_view> {
      input sin = in;
      // Much better would be:
      // while (const auto r = p(sin)) {
      // But that hits a weird bug in clang (as of 08/2022)
      auto r = p(in);
      while (r) {
        sin = r.remainder();
        r = p(sin);
      }
      return output { in.peek(sin.distance(in)), sin };
    };
  }

  template<typename T, parser P, typename Fn>
  inline constexpr auto agg(T && init, P && p, Fn && fn) noexcept {
    return [t = std::forward<T>(init), p = std::forward<P>(p), fn = std::forward<Fn>(fn)](const input in) noexcept {
      auto res = t;
      input sin = in;
      auto r = p(sin); // See `whilst`
      while (r) {
        if constexpr (std::is_member_function_pointer_v<Fn>) {
          (res.*fn)(*r);
        } else {
          res = fn(res, *r);
        }
        sin = r.remainder();
        r = p(sin);
      }
      return r.with_value(res);
    };
  }
}
