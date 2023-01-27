export module koopa:str;
import jute;
import traits;

export namespace koopa {
  class str {
    jute::view m_view;
    bool m_heap = false;

    [[nodiscard]] static constexpr jute::view clone(jute::view sv) noexcept {
      char * data = new char[sv.size()]; // NOLINT
      jute::view res { data, sv.size() };
      for (auto c : sv) {
        *data++ = c;
      }
      return res;
    }

  public:
    constexpr str() noexcept = default;
    constexpr str(jute::view v, bool h) noexcept : m_view { v }, m_heap { h } {
    }
    constexpr explicit str(jute::view sv) noexcept : str { clone(sv), true } {
    }

    constexpr ~str() noexcept {
      if (m_heap) delete[] m_view.data();
    }

    constexpr str(const str & o) noexcept : str { clone(*o), true } {
    }
    constexpr str(str && o) noexcept : str { o.m_view, o.m_heap } {
      o.m_heap = false;
    }
    constexpr str & operator=(const str & o) noexcept {
      if (this == &o) return *this;
      if (m_heap) delete[] m_view.data();
      m_view = clone(*o);
      m_heap = true;
      return *this;
    }
    constexpr str & operator=(str && o) noexcept {
      if (this == &o) return *this;
      m_view = o.m_view;
      m_heap = o.m_heap;
      o.m_heap = false;
      return *this;
    }

    [[nodiscard]] constexpr jute::view operator*() const noexcept {
      return m_view;
    }

    [[nodiscard]] constexpr str operator+(jute::view o) const {
      const auto len = m_view.size() + o.size();
      auto * data = new char[len]; // NOLINT
      auto view = jute::view { data, len };
      for (auto c : m_view) {
        *data++ = c;
      }
      for (auto c : o) {
        *data++ = c;
      }
      return str { view, true };
    }

    [[nodiscard]] constexpr str operator+(const str & o) const {
      return *this + *o;
    }
    [[nodiscard]] constexpr str operator+(char c) const {
      return *this + jute::view { &c, 1 };
    }

    [[nodiscard]] constexpr bool operator==(const str & o) const noexcept {
      return **this == *o;
    }
  };

  [[nodiscard]] inline constexpr str operator"" _ks(const char * c, traits::size_t len) noexcept {
    return str { jute::view { c, len }, false };
  }
}
