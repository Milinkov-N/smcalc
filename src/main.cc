#include <iostream>

#include "args.h"
#include "lexer.h"
#include "rpn.h"

size_t bytes_allocated = 0;

constexpr auto USAGE_MSG = R"(Usage: smcalc  <expression> [OPTIONS...]

    OPTIONS:
        --var string        Sets variable for usage in expression (format:
                            <name>:<value>)
        -B, --show-bytes    Shows how many bytes program is used
        -T, --show-tokens   Shows raw, unparsed tokens
        -R, --show-rpn      Shows parsed tokens (Reverse Polish Notation)
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
  Lexer l(args.expr());
  auto vars = args.find_all_variables();
  auto raw_tokens = l.collect();

  try {
    auto rpn = rpn::parse(raw_tokens);
    auto parsed_vars = utils::parse_variables(vars);

    if (args.find_flag("-T").first || args.find_flag("--show-tokens").first)
      utils::tokens_dbg(raw_tokens, "Raw Tokens");

    if (args.find_flag("-R").first || args.find_flag("--show-rpn").first)
      utils::tokens_dbg(rpn, "Parsed Tokens");

    std::cout << "Variables:\n";
    for (auto& v : parsed_vars)
      std::cout << "  " << v.first << " = " << v.second << std::endl;
  } catch (std::logic_error& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  if (args.find_flag("-B").first || args.find_flag("--show-bytes").first)
    std::cout << "\n\nBytes used: " << bytes_allocated << std::endl;
  return 0;
}
