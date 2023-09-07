#pragma once

#include <map>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace alias {
using raw_vars = std::vector<std::string_view>;
using parsed_vars = std::map<std::string_view, double>;
}  // namespace alias

class Arg {
  // constructors
 public:
  Arg();
  explicit Arg(char* str);
  Arg(const Arg& other) = default;
  Arg(Arg&& other) noexcept = default;
  ~Arg();

  // operators
 public:
  bool operator==(const Arg& other) const;

  // methods
 public:
  const std::string_view& str() const { return m_str; }
  bool is_flag() const;

 private:
  std::string_view m_str{};
};

std::ostream& operator<<(std::ostream& s, const Arg& arg);

class Args {
 public:
  Args(int argc, char* argv[]);
  ~Args();

  const std::string_view& expr() const { return m_expr; }

  std::pair<bool, int> find_flag(std::string_view flag) const;

  alias::raw_vars find_all_variables() const;

 private:
  std::vector<Arg> m_args{};
  std::string_view m_expr{""};
};

namespace utils {
inline const char* bool_to_string(bool b) { return b ? "true" : "false"; }
alias::parsed_vars parse_variables(const alias::raw_vars& vars);
}  // namespace utils
