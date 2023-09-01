#include <iostream>

#include "args.h"

constexpr auto USAGE_MSG = R"(Usage: smcalc  <expression> [OPTIONS...]

    OPTIONS:
        --var       Sets variable for usage in expression (format:
                    <name>:<value>)
)";

auto main(int argc, char* argv[]) -> int {
  if (argc < 2) {
    std::cout << USAGE_MSG;
    return 0;
  }

  Args args(argc, argv);

  auto vars = args.find_all_variables();

  try {
    auto parsed = utils::parse_variables(vars);

    for (auto& v : parsed)
      std::cout << v.first << " = " << v.second << std::endl;
  } catch (std::logic_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  //   std::cout << name << " = " << value << std::endl;
  return 0;
}
