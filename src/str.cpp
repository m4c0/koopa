#include "koopa/str.hpp"

using namespace koopa;
using namespace std::string_view_literals;

static_assert((*""_s).empty());
static_assert(*"a"_s == "a");
static_assert("test"_s == "test"_s);
static_assert("aaa"_s + "bb"_s == "aaabb"_s);
static_assert(*(std::move("a"_s)) == "a");
static_assert(str { "asd"sv } == "asd"_s);
static_assert("aaa"_s + "bb"sv + "c" == "aaabbc"_s);
