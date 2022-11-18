module;
#include <type_traits>

export module koopa:type_traits;

export namespace koopa {
  class input;

  template<typename A, typename B>
  concept not_same_as = !
  std::is_same_v<A, B>;

  template<typename P>
  concept parser = requires(P p, const input i) {
                     { p(i) };
                   };

  template<parser P>
  using result_of = typename std::invoke_result_t<P, const input>::type;
}
