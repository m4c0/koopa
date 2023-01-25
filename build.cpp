#include "ecow.hpp"

using namespace ecow;

int main(int argc, char ** argv) {
  seq all { "all" };

  auto m = all.add_unit<mod>("koopa");
  m->add_part("str");
  m->add_part("type_traits");
  m->add_part("io");
  m->add_part("binary");
  m->add_part("combiner");
  m->add_part("constants");
  m->add_part("matcher");

  all.add_unit<>("binary");
  all.add_unit<>("combiner");
  all.add_unit<>("dummy");
  all.add_unit<>("io");
  all.add_unit<>("matcher");
  all.add_unit<>("str");

  return run_main(all, argc, argv);
}
