#pragma once

#include <string_view>

#include "args.h"
#include "lexer.h"

class Calculator {
 public:
  Calculator(std::string_view expr, alias::parsed_vars&& vars);

  double eval();

 private:
  Lexer m_lexer;
  alias::parsed_vars m_vars;
};
