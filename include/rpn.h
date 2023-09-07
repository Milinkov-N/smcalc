#include <map>
#include <string_view>

#include "Lexer.h"

namespace rpn {
alias::tokens parse(const alias::tokens& tokens);
double evaluate(const alias::tokens& rpn,
                const std::map<std::string_view, double>& vars);
}  // namespace rpn
