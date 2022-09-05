#include "koopa/binary.hpp"
#include "koopa/combiner.hpp"

#include <bit>

using namespace koopa;

// Some tests are easier if defined as LE
static_assert(std::endian::native == std::endian::little);

static constexpr const auto t_u8 = take_u8();
static_assert(t_u8(""_i) == fail<uint8_t>("eof while expecting u8", ""_i));
static_assert(t_u8("a"_i) == output<uint8_t> { 'a', ""_i });
static_assert(t_u8("bc"_i) == output<uint8_t> { 'b', "c"_i });

static constexpr const auto t_u16le = take_u16le();
static_assert(t_u16le(""_i) == fail<uint16_t>("eof while expecting u16le", ""_i));
static_assert(t_u16le("a"_i) == fail<uint16_t>("eof while expecting u16le", "a"_i));
static_assert(t_u16le("\1\1"_i) == output<uint16_t> { 257, ""_i });         // NOLINT
static_assert(t_u16le("\2\1c"_i) == output<uint16_t> { 258, "c"_i });       // NOLINT
static_assert(t_u16le("🍌"_i) == output<uint16_t> { 0x9FF0, "\x8d\x8c"_i }); // NOLINT

static constexpr const auto t_u32le = take_u32le();
static_assert(t_u32le(""_i) == fail<uint32_t>("eof while expecting u32le", ""_i));
static_assert(t_u32le("aaa"_i) == fail<uint32_t>("eof while expecting u32le", "aaa"_i));
static_assert(t_u32le("\0\0\0\1"_i) == output<uint32_t> { 1 << 24, ""_i });      // NOLINT
static_assert(t_u32le("\4\3\2\1c"_i) == output<uint32_t> { 0x01020304, "c"_i }); // NOLINT
static_assert(t_u32le("🍌c"_i) == output<uint32_t> { 0x8C8D9FF0, "c"_i });        // NOLINT

static constexpr const auto t_mu8 = match_u8('n');
static_assert(t_mu8("meh"_i) == fail<uint8_t>("mismatched u8", "meh"_i)); // NOLINT
static_assert(t_mu8("neh"_i) == output<uint8_t> { 'n', "eh"_i });         // NOLINT

static constexpr const auto t_mu16le = match_u16le(257);                               // NOLINT
static_assert(t_mu16le("\0\1eh"_i) == fail<uint16_t>("mismatched u16le", "\0\1eh"_i)); // NOLINT
static_assert(t_mu16le("\1\1eh"_i) == output<uint16_t> { 257, "eh"_i });               // NOLINT

static constexpr const auto t_mu32le = match_u32le(0x01020304);                              // NOLINT
static_assert(t_mu32le("\1\1\1\1h"_i) == fail<uint32_t>("mismatched u32le", "\1\1\1\1h"_i)); // NOLINT
static_assert(t_mu32le("\4\3\2\1h"_i) == output<uint32_t> { 0x01020304, "h"_i });            // NOLINT

struct wad_header {
  uint32_t num_lumps;
  uint32_t info_table_ofs;
};
static constexpr const auto poc_wad_header =
    (match_u32le('DAWI'), combine(take_u32le(), take_u32le(), [](auto nlumps, auto tblofs) noexcept {
       return wad_header { nlumps, tblofs };
     }));
static_assert(!poc_wad_header("PWAD\3\0\0\0\1\0\0\0"_i));
static_assert((*poc_wad_header("IWAD\3\0\0\0\1\0\0\0"_i)).num_lumps == 3);
static_assert((*poc_wad_header("IWAD\3\0\0\0\1\0\0\0"_i)).info_table_ofs == 1);
