#include <iostream>

#include "args.h"
#include "lexer.h"

size_t bytes_allocated = 0;

constexpr auto USAGE_MSG = R"(Usage: smcalc  <expression> [OPTIONS...]

    OPTIONS:
        --var string        Sets variable for usage in expression (format:
                            <name>:<value>)
        -B, --show-bytes    Shows how many bytes program is used
        -T, --show-tokens   Shows raw, unparsed tokens
)";

void* operator new(size_t size) {
  bytes_allocated += size;
  return malloc(size);
}

auto main(int argc, char* argv[]) -> int {
  if (argc < 2) {
    std::cout << USAGE_MSG;
    return 0;
  }

  Args args(argc, argv);

  auto vars = args.find_all_variables();

  Lexer l(args.expr());

  if (args.find_flag("-T").first || args.find_flag("--show-tokens").first) {
    std::cout << "raw tokens:\n";
    utils::tokens_dbg(l.collect());
  }

  try {
    auto parsed = utils::parse_variables(vars);

    for (auto& v : parsed)
      std::cout << v.first << " = " << v.second << std::endl;
  } catch (std::logic_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  if (args.find_flag("-B").first || args.find_flag("--show-bytes").first)
    std::cout << "\n\n\nBytes used: " << bytes_allocated << std::endl;
  return 0;
}
