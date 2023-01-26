import koopa;

#include <optional>
#include <string_view>

using namespace koopa;
using namespace std::string_view_literals;

static constexpr const auto p = combine(match('a'), match("b"), [](char a, std::string_view b) {
  return a == 'a' && b == "b";
});

static_assert(p(""_i) == fail<bool>("eof while waiting for 'a'", ""_i));
static_assert(p("a"_i) == fail<bool>("expecting 'b' got ''", ""_i));
static_assert(p("b"_i) == fail<bool>("expecting 'a' got 'b'", "b"_i));
static_assert(p("ax"_i) == fail<bool>("expecting 'b' got 'x'", "x"_i));
static_assert(p("ab"_i) == output { true, ""_i });
static_assert(p("abc"_i) == output { true, "c"_i });

static constexpr const auto q = (match('a'), match('b'), match('c'));

static_assert(q(""_i) == fail<char>("eof while waiting for 'a'", ""_i));
static_assert(q("a"_i) == fail<char>("eof while waiting for 'b'", ""_i));
static_assert(q("b"_i) == fail<char>("expecting 'a' got 'b'", "b"_i));
static_assert(q("ab"_i) == fail<char>("eof while waiting for 'c'", ""_i));
static_assert(q("abc"_i) == output { 'c', ""_i });
static_assert(q("abcd"_i) == output { 'c', "d"_i });

static constexpr const auto r = until(match('a'));

static_assert(r(""_i) == output { ""sv, ""_i });
static_assert(r("a"_i) == output { ""sv, "a"_i });
static_assert(r("b"_i) == output { "b"sv, ""_i });
static_assert(r("ab"_i) == output { ""sv, "ab"_i });
static_assert(r("bbbab"_i) == output { "bbb"sv, "ab"_i });

static constexpr const auto s = whilst(match("ab"));

static_assert(s(""_i) == output { ""sv, ""_i });
static_assert(s("ab"_i) == output { "ab"sv, ""_i });
static_assert(s("ababab"_i) == output { "ababab"sv, ""_i });
static_assert(s("b"_i) == output { ""sv, "b"_i });
static_assert(s("ababa"_i) == output { "abab"sv, "a"_i });

static constexpr const auto t = match('a') - match("b");

static_assert(t(""_i) == fail<char>("eof while waiting for 'a'", ""_i));
static_assert(t("a"_i) == fail<char>("expecting 'b' got ''", ""_i));
static_assert(t("b"_i) == fail<char>("expecting 'a' got 'b'", "b"_i));
static_assert(t("ab"_i) == output { 'a', ""_i });
static_assert(t("aba"_i) == output { 'a', "a"_i });

static constexpr const auto t_fmap_0 = fmap(
    [](char c) noexcept {
      return c - '0';
    },
    match('1'));

static_assert(t_fmap_0(""_i) == fail<int>("eof while waiting for '1'", ""_i));
static_assert(t_fmap_0("1x"_i) == output { 1, "x"_i });

static constexpr const auto t_fmap_1 = fmap(&std::string_view::size, match("uga"));
static_assert(t_fmap_1("buga"_i) == fail<size_t>("expecting 'uga' got 'bug'", "buga"_i));
static_assert(t_fmap_1("ugauga"_i) == output { size_t { 3 }, "uga"_i });

static constexpr const auto w = map(
    [](const output<char> & o) noexcept -> output<int> {
      if (o) return o.with_value(3);
      return fail<int>("ok", o.remainder());
    },
    match('1'));

static_assert(w(""_i) == fail<int>("ok", ""_i));
static_assert(w("1x"_i) == output { 3, "x"_i });

static constexpr const auto x = match('a') | match('b');

static_assert(x(""_i) == fail<char>("expecting different alternative", ""_i));
static_assert(x("a"_i) == output { 'a', ""_i });
static_assert(x("b"_i) == output { 'b', ""_i });
static_assert(x("c"_i) == fail<char>("expecting different alternative", "c"_i));
static_assert(x("ab"_i) == output { 'a', "b"_i });

static constexpr const auto y = ifelse(match('a'), constant(0), constant(1));

static_assert(y(""_i) == output { 1, ""_i });
static_assert(y("a"_i) == output { 0, ""_i });
static_assert(y("ab"_i) == output { 0, "b"_i });
static_assert(y("b"_i) == output { 1, "b"_i });

static constexpr const auto t_agg_0 = agg(0, match('a'), [](auto a, auto b) noexcept {
  return a + (b == 'a' ? 1 : 0);
});
static_assert(t_agg_0(""_i) == output { 0, ""_i });
static_assert(t_agg_0("b"_i) == output { 0, "b"_i });
static_assert(t_agg_0("aa"_i) == output { 2, ""_i });
static_assert(t_agg_0("aaabb"_i) == output { 3, "bb"_i });

class c_agg_v {
  int m_v;

public:
  explicit constexpr c_agg_v(int v) noexcept : m_v { v } {
  }
  constexpr void add(char v) noexcept {
    m_v += v == 'b' ? 1 : 0;
  }
  [[nodiscard]] constexpr bool operator==(const c_agg_v & o) const noexcept {
    return o.m_v == m_v;
  }
};
static constexpr const auto t_agg_1 = agg(c_agg_v { 0 }, match('b'), &c_agg_v::add);
static_assert(t_agg_1("bba"_i) == output { c_agg_v { 2 }, "a"_i });
