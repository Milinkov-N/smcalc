#include "calc.h"

#include "rpn.h"

Calculator::Calculator(std::string_view expr, alias::parsed_vars&& vars)
    : m_lexer(expr), m_vars(std::move(vars)) {}

double Calculator::eval() {
  auto raw_tokens = m_lexer.collect();
  auto rpn = rpn::parse(raw_tokens);
  return rpn::evaluate(rpn, m_vars);
}
