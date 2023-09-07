#include "args.h"

#include <exception>
#include <limits>
#include <sstream>

Arg::Arg() : m_str("") {}

Arg::Arg(char* str) {
  if (str == nullptr) throw std::invalid_argument("Arg: str was null");

  m_str = str;
}

Arg::~Arg() {}

bool Arg::is_flag() const {
  // Minimum characters required to be a flag: -<char>
  if (m_str.size() < 2) return false;

  const char* ptr = m_str.data();

  return !strncmp(ptr, "-", 1) || !strncmp(ptr, "--", 2);
}

bool Arg::operator==(const Arg& other) const {
  return !m_str.compare(other.m_str.data());
}

std::ostream& operator<<(std::ostream& s, const Arg& arg) {
  s << arg.str();
  return s;
}

Args::Args(int argc, char* argv[]) {
  if (argc < 1) throw std::invalid_argument("Args: argc was less than 1");
  if (argv[1] != nullptr) m_expr = argv[1];
  for (int i = 2; i != argc; ++i) m_args.emplace_back(argv[i]);
}

Args::~Args() {}

std::pair<bool, int> Args::find_flag(std::string_view flag) const {
  bool isset = false;
  int idx = 0;

  for (auto it = m_args.cbegin(); it != m_args.cend(); ++it, ++idx)
    if ((*it).str() == flag) {
      isset = true;
      break;
    }

  return {isset, idx};
}

alias::raw_vars Args::find_all_variables() const {
  alias::raw_vars vars;

  for (auto it = m_args.cbegin(); it != m_args.cend(); ++it)
    if ((*it).str() == "--var" && it + 1 != m_args.cend())
      vars.emplace_back((*(++it)).str());

  return vars;
}

alias::parsed_vars utils::parse_variables(const alias::raw_vars& vars) {
  alias::parsed_vars parsed_vars{};

  for (auto& var : vars) {
    std::size_t sep = var.find(':');

    if (std::numeric_limits<size_t>::max() == sep) {
      std::stringstream ss;

      ss << "smcalc: invalid variable '" << var << "' assignment.\n "
         << "\tDid you mean '" << var << ":<value>'?";

      throw std::logic_error(ss.str());
    }

    auto name = var.substr(0, sep);
    auto value = var.substr(sep + 1);

    parsed_vars.emplace(name, std::atof(value.data()));
  }

  return parsed_vars;
}
