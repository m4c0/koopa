#include "koopa/constants.hpp"

using namespace koopa;

static constexpr const auto k = constant(3);
static_assert(k(""_i) == output { 3, ""_i });
static_assert(k("abc"_i) == output { 3, "abc"_i });

static constexpr const auto f = failure<int>("meh");
static_assert(f(""_i) == fail<int>("meh", ""_i));
static_assert(f("abc"_i) == fail<int>("meh", "abc"_i));

static_assert(eof(""_i) == output<nullptr_t> { nullptr, ""_i });
static_assert(eof("a"_i) == fail<nullptr_t>("expecting eof", "a"_i));
