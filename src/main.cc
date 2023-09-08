#include <iostream>
#include <map>

#include "args.h"
#include "calc.h"
#include "lexer.h"
#include "rpn.h"

constexpr auto USAGE_MSG = R"(Usage: smcalc  <expression> [OPTIONS...]

    OPTIONS:
        --var string        Sets variable for usage in expression (format:
                            <name>:<value>)
)";

auto main(int argc, char* argv[]) -> int {
  if (argc < 2) {
    std::cout << USAGE_MSG;
    return 0;
  }

  Args args(argc, argv);
  auto vars = args.find_all_variables();
  auto parsed = utils::parse_variables(vars);
  Calculator calc(args.expr(), std::move(parsed));

  try {
    std::cout << "Result: " << calc.eval() << std::endl;
  } catch (std::logic_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
