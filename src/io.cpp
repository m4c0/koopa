#include "koopa/io.hpp"

using namespace koopa;

static_assert(!input { "" });
static_assert(input { "a" });

static_assert("alpha"_i.peek() == 'a');
static_assert("alpha"_i.peek(3) == "alp");
static_assert("alpha"_i.take(2) == "pha"_i);
static_assert([] {
  const auto i = "alpha"_i;
  return i.take(2).distance(i) == 2;
}());

static_assert(output { 'a', "b"_i });
static_assert(!fail<char>("mismatched char", "b"_i));

static_assert(*output { 'a', "b"_i } == 'a');

static_assert(output { 'a', "b"_i }.remainder() == "b"_i);

static_assert(fail<char>("test", ""_i).error() == error { "test"_s });

static_assert(output { 'a', "b"_i } == output { 'a', "b"_i });
static_assert(output { '0', "b"_i } != output { 'a', "b"_i });
static_assert(output { 'a', "b0"_i } != output { 'a', "b"_i });
static_assert(fail<char>("ok", "bb"_i) == fail<char>("ok", "bb"_i));
static_assert(fail<char>("ok", "cc"_i) != fail<char>("ok", "bb"_i));
static_assert(fail<char>("nok", "bb"_i) != fail<char>("ok", "bb"_i));

static_assert(output { 'a', "b"_i }.with_value('c') == output { 'c', "b"_i });
static_assert(fail<char>("ok", "bb"_i).with_error("nok") == fail<char>("nok", "bb"_i));
static_assert(fail<char>("ok", "bb"_i).with_error_type<int>() == fail<int>("ok", "bb"_i));
static_assert(fail<char>("ok", "bb"_i).with_cause<int>("ish") == fail<int>("ok\ncaused by: ish", "bb"_i));
