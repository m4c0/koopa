module;
#include <type_traits>

export module koopa:type_traits;
import traits;

using namespace traits;

namespace koopa {
  class input;
  template<typename>
  class output;

  template<typename T>
  struct is_output_type : false_t {};
  template<typename T>
  struct is_output_type<output<T>> : true_t {};

  template<typename O>
  concept is_output = is_output_type<O>::value;

  template<typename P>
  concept parser = requires(P p, const input i) {
                     { p(i) } -> is_output;
                   };

  template<parser P>
  using result_of = typename std::invoke_result_t<P, const input>::type;
}
