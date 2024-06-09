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

#include "../hash/fnv1a.h"
#include "../json/json.h"
#include "details/format.h"

namespace sh {
enum e_localization : size_t {
  _unknown = 0,
  _ru,
  _en,
  _cn,
  _count,
};

class c_locale final {
  std::map<hash_t, std::string> m_data;

 public:
  template <typename _container>
  void from_container(_container&& cont) {
    for (auto&& [k, v] : cont) add(k, v);
  }

  void to_json(nlohmann::json& json) { json = m_data; }

  void from_json(nlohmann::json& json) {
    m_data = json.get<std::map<hash_t, std::string>>();
  }

  void add(hash_t synonim, const std::string& value) {
    if (m_data.contains(synonim)) {
      /* @todo: Here we need logging for this exception */
      throw std::runtime_error{details::format(
          "in [",
          details::source_location_format(std::source_location::current()),
          "] cannot add second value as value='", value, "' for synonym='",
          synonim, '\'')};
    }

    m_data.emplace(synonim, value);
  }

  std::string get(hash_t const key) {
    for (auto&& data : m_data) {
      if (data.first != key) continue;

      return data.second;
    }

    return "failed to find localization";
  }

  std::string_view operator[](hash_t const s) const {
    auto const it = m_data.find(s);
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

class c_localization final {
  std::array<c_locale, _count> m_locals{};
  c_localization() = default;

 public:
  static c_localization& instance() {
    static c_localization loc;
    return loc;
  }

  c_locale& operator[](e_localization const lang) & {
    return m_locals.at(lang);
  }
};

}  // namespace sh
