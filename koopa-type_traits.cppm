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
  using result_of = typename call_result_t<P, const input>::type;

  template<typename T, typename I>
  struct mapper_traits {
    using result_t = call_result_t<T, I>;
    static constexpr auto invoke(auto fn, auto in) {
      return fn(in);
    }
  };

  template<typename M, typename O>
  struct mapper_traits<M(O::*), O> {
    using T = M(O::*);
    using result_t = decltype((declval<O>().*declval<T>())());
    static constexpr auto invoke(M(O::*fn), auto in) {
      return (in.*fn)();
    }
  };
}

namespace {
  using namespace koopa;

  struct dummy {
    long test() const noexcept;
    static long stest(int) noexcept;
  };
  static_assert(is_same_v<long, mapper_traits<decltype(&dummy::stest), int>::result_t>);
  static_assert(is_same_v<long, mapper_traits<decltype(&dummy::test), dummy>::result_t>);
}
