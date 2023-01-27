import koopa;
import jute;
import traits;

using namespace koopa;
using namespace jute::literals;

static_assert((*""_ks).size() == 0);
static_assert(*"a"_ks == "a");
static_assert("test"_ks == "test"_ks);
static_assert("aaa"_ks + "bb"_ks == "aaabb"_ks);
static_assert(*(traits::move("a"_ks)) == "a");
static_assert(str { "asd"_s } == "asd"_ks);
static_assert("aaa"_ks + "bb"_s + "c" + 'd' == "aaabbcd"_ks);
