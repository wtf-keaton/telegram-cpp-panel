/**
 * Format tools
 *
 *
 */

#pragma once
#include <source_location>
#include <string>

namespace sh {
namespace details {
template <typename... _ts>
constexpr std::string format(_ts&&... ts) {
  std::stringstream ss;
  (ss << ... << std::forward<_ts>(ts));
  return ss.str();
}
constexpr std::string source_location_format(std::source_location const& lc) {
  return format("filename'", lc.file_name(), "' in function='",
                lc.function_name(), "' at line ", lc.line(), " column ",
                lc.column());
}

}  // namespace details
}  // namespace sh
