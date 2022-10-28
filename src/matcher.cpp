#include "koopa/matcher.hpp"

using namespace koopa;
using namespace std::string_view_literals;

static_assert(any()(""_i) == fail<char>("eof while waiting for any data", ""_i));
static_assert(any()("a"_i) == output { 'a', ""_i });
static_assert(any()("ba"_i) == output { 'b', "a"_i });

static_assert(match('a')(""_i) == fail<char>("eof while waiting for 'a'", ""_i));
static_assert(match('a')("a"_i) == output { 'a', ""_i });
static_assert(match('a')("aa"_i) == output { 'a', "a"_i });
static_assert(match('b')("bc"_i) == output { 'b', "c"_i });
static_assert(match('a')("b"_i) == fail<char>("expecting 'a' got 'b'", "b"_i));

static_assert(match("cca")(""_i) == fail<std::string_view>("expecting 'cca' got ''", ""_i));
static_assert(match("cca")("cc"_i) == fail<std::string_view>("expecting 'cca' got 'cc'", "cc"_i));
static_assert(match("cca")("ccb"_i) == fail<std::string_view>("expecting 'cca' got 'ccb'", "ccb"_i));
static_assert(match("cca")("cca"_i) == output { "cca"sv, ""_i });
static_assert(match("cca")("ccaa"_i) == output { "cca"sv, "a"_i });
static_assert(match("ccb")("ccbc"_i) == output { "ccb"sv, "c"_i });
