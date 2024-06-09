/*
 *
 * I do not want to separate this file to .cpp and .h ones, because this one is
 * not big as it might be. Need some examples\tests. Perhaps, I may do it later.
 * Advice:
 * Maybe we should use std::flat_map since C++23, if there won't be a lot of
 * data(less than about 64). Add hash for string literars.
 */

#pragma once

#include <array>
#include <map>
#include <source_location>
#include <sstream>    // strinstream
#include <stdexcept>  // runtime_error
#include <string>
#include <string_view>
#include <utility>  // forward

namespace __details {
template <typename... Ts>
constexpr std::string format(Ts&&... ts) {
  std::stringstream ss;
  (ss << ... << std::forward<Ts>(ts));
  return ss.str();
}
constexpr std::string source_location_format(std::source_location lc) {
  return format("filename'", lc.file_name(), "' in function='",
                lc.function_name(), "' at line ", lc.line(), " column ",
                lc.column());
}
}  // namespace __details

namespace sh {

enum localization_t : size_t {
  null_loc = 0,
  ru,
  en,
  zh,  // chinese
  max_loc,
};

class locale final {
 private:
  std::map<std::string, std::string> m_data;

 public:
  template <typename Container>
  void from_container(Container&& cont) {
    for (auto&& [k, v] : cont) add(k, v);
  }

  void add(const std::string& synonim, const std::string& value) {
    if (m_data.find(synonim) != m_data.end()) {
      /* TODO: Here we need logging for this exception */
      throw std::runtime_error{__details::format(
          "in [",
          __details::source_location_format(std::source_location::current()),
          "] cannot add second value as value='", value, "' for synonym='",
          synonim, '\'')};
    }
    m_data.emplace(synonim, value);
  }

  std::string_view operator[](const std::string& s) const {
    auto it = m_data.find(s);
    if (it == m_data.end()) return {};
    return std::string_view{it->second};
  }

  auto begin() noexcept { return m_data.begin(); }
  auto end() noexcept { return m_data.end(); }
  auto cbegin() const noexcept { return m_data.cbegin(); }
  auto cend() const noexcept { return m_data.cend(); }
  auto rbegin() noexcept { return m_data.rbegin(); }
  auto rend() noexcept { return m_data.rend(); }
  auto crbegin() const noexcept { return m_data.crbegin(); }
  auto crend() const noexcept { return m_data.crend(); }
  bool empty() const noexcept { return m_data.empty(); }
  void clear() noexcept { return m_data.clear(); }
};

class localization final {
 private:
  std::array<locale, static_cast<size_t>(localization_t::max_loc)> m_locals{};
  localization() = default;

 public:
  static localization& instance() {
    static localization loc;
    return loc;
  }

  locale& operator[](localization_t lang) & {
    return m_locals.at(static_cast<size_t>(lang));
  }
};

}  // namespace sh
