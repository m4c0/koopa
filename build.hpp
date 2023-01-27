#pragma once

#include "../jute/build.hpp"
#include "../traits/build.hpp"
#include "ecow.hpp"

auto koopa() {
  using namespace ecow;

  auto m = unit::create<mod>("koopa");
  m->add_wsdep("jute", jute());
  m->add_wsdep("traits", traits());
  m->add_part("str");
  m->add_part("type_traits");
  m->add_part("io");
  m->add_part("binary");
  m->add_part("combiner");
  m->add_part("constants");
  m->add_part("matcher");
  return m;
}
