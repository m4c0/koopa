#include "build.hpp"

using namespace ecow;

int main(int argc, char ** argv) {
  seq all { "all" };

  all.add_ref(koopa());

  all.add_unit<>("binary");
  all.add_unit<>("combiner");
  all.add_unit<>("dummy");
  all.add_unit<>("io");
  all.add_unit<>("matcher");
  all.add_unit<>("str");

  return run_main(all, argc, argv);
}
