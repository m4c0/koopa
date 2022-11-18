module;
#include <string_view>

export module koopa:str;

export namespace koopa {
  class str {
    const char * m_data = "";
    size_t m_len = 0;
    bool m_heap = false;

    [[nodiscard]] static constexpr const char * clone(std::string_view sv) noexcept {
      char * data = new char[sv.size()]; // NOLINT
      std::copy(sv.begin(), sv.end(), data);
      return data;
    }

  public:
    constexpr str() noexcept = default;
    constexpr str(const char * c, size_t l, bool h) noexcept : m_data { c }, m_len { l }, m_heap { h } {
    }
    constexpr explicit str(std::string_view sv) noexcept : str { clone(sv), sv.size(), true } {
    }

    constexpr ~str() noexcept {
      if (m_heap) delete[] m_data;
    }

    constexpr str(const str & o) noexcept : str { clone(*o), o.m_len, true } {
    }
    constexpr str(str && o) noexcept : str { o.m_data, o.m_len, o.m_heap } {
      o.m_data = nullptr;
      o.m_heap = false;
    }
    constexpr str & operator=(const str & o) noexcept {
      if (this == &o) return *this;
      if (m_heap) delete[] m_data;
      m_data = clone(*o);
      m_len = o.m_len;
      m_heap = true;
      return *this;
    }
    constexpr str & operator=(str && o) noexcept {
      if (this == &o) return *this;
      m_data = o.m_data;
      m_len = o.m_len;
      m_heap = o.m_heap;
      o.m_data = nullptr;
      o.m_heap = false;
      return *this;
    }

    [[nodiscard]] constexpr std::string_view operator*() const noexcept {
      return std::string_view { m_data, m_len };
    }

    [[nodiscard]] constexpr str operator+(std::string_view o) const {
      const auto len = m_len + o.size();
      auto * data = new char[len];                 // NOLINT
      std::copy(m_data, m_data + m_len, data);     // NOLINT
      std::copy(o.begin(), o.end(), data + m_len); // NOLINT
      return str { data, len, true };
    }

    [[nodiscard]] constexpr str operator+(const str & o) const {
      return *this + *o;
    }
    [[nodiscard]] constexpr str operator+(char c) const {
      return *this + str { &c, 1, false };
    }

    [[nodiscard]] constexpr bool operator==(const str & o) const noexcept {
      return **this == *o;
    }
  };

  [[nodiscard]] inline constexpr str operator"" _s(const char * c, size_t len) noexcept {
    return str { c, len, false };
  }
}
