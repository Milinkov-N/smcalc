#pragma once

#include <ostream>
#include <string_view>
#include <vector>

class Token;

namespace alias {
using tokens = std::vector<Token>;
}  // namespace alias

class Token {
 public:
  enum class Kind {
    StartStream = -3,
    EndStream,
    Invalid,
    Whitespace,
    Number,
    Variable,
    OpenBrace,
    CloseBrace,
    PlusOp,
    MinusOp,
    MulOp,
    DivideOp,
    ModuloOp,
    ExpOp,
    Negate,
    Function,
    Ident,
  };

 public:
  Token();
  Token(Kind kind);
  Token(Kind kind, std::string_view value);
  Token(const Token& t) = default;
  Token(Token&& t) noexcept = default;
  ~Token();

 public:
  Token& operator=(const Token& t) = default;
  Token& operator=(Token&& t) noexcept = default;
  bool operator==(const Token& other) const;
  bool operator!=(const Token& other) const;

 public:
  Kind kind() const { return m_kind; }
  bool is_number() const { return m_kind == Token::Kind::Number; }
  bool is_operator() const;
  bool is_open_brace() const { return m_kind == Token::Kind::OpenBrace; }
  bool is_close_brace() const { return m_kind == Token::Kind::CloseBrace; }
  bool is_ident() const { return m_kind == Token::Kind::Ident; }
  const std::string_view& val() const { return m_value; }

  const char* to_cstr() const;

 private:
  Kind m_kind;
  std::string_view m_value;
};

std::ostream& operator<<(std::ostream& s, const Token& t);

class Lexer {
 public:
  Lexer(std::string_view expr);
  ~Lexer();

 public:
  Token next();
  alias::tokens collect();

  // utility functions
 private:
  Token _digit();
  Token _operator();
  Token _ident();

 private:
  std::string_view m_expr;
  std::string_view::iterator m_it;
  std::string_view::iterator m_end;
  Token m_prev;
};

namespace utils {
inline bool isop(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' ||
         c == '(' || c == ')';
}

void tokens_dbg(alias::tokens& tokens, std::string_view name = "Tokens");
}  // namespace utils
