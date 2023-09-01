#include "lexer.h"

#include <exception>
#include <iostream>
#include <sstream>

Token::Token() : m_kind(Token::Kind::Invalid), m_value("") {}

Token::Token(Kind kind) : m_kind(kind), m_value("") {}

#ifndef _UNOPTIMIZED
Token::Token(Kind kind, std::string_view value)
    : m_kind(kind), m_value(value) {}
#endif

Token::~Token() {}

bool Token::is_operator() const {
  return m_kind == Kind::Negate || m_kind == Kind::PlusOp ||
         m_kind == Kind::MinusOp || m_kind == Kind::MulOp ||
         m_kind == Kind::DivideOp || m_kind == Kind::ModuloOp ||
         m_kind == Kind::ExpOp || m_kind == Kind::OpenBrace ||
         m_kind == Kind::CloseBrace;
}

const char* Token::to_cstr() const {
  switch (m_kind) {
    case Token::Kind::StartStream:
      return "StartStream";
    case Token::Kind::EndStream:
      return "EndStream";
    case Token::Kind::Invalid:
      return "Invalid";
    case Token::Kind::Whitespace:
      return "Whitespace";
    case Token::Kind::Number:
      return "Number";
    case Token::Kind::OpenBrace:
      return "OpenBracket";
    case Token::Kind::CloseBrace:
      return "CloseBracket";
    case Token::Kind::PlusOp:
      return "PlusOp";
    case Token::Kind::MinusOp:
      return "MinusOp";
    case Token::Kind::MulOp:
      return "MulOp";
    case Token::Kind::DivideOp:
      return "DivideOp";
    case Token::Kind::ModuloOp:
      return "ModuloOp";
    case Token::Kind::ExpOp:
      return "ExpOp";
    case Token::Kind::Negate:
      return "Negate";
    case Token::Kind::Ident:
      return "Ident";
    default:
      return "???";
  }
}

std::ostream& operator<<(std::ostream& s, const Token& t) {
  s << "Token::" << t.to_cstr();

  switch (t.kind()) {
    case Token::Kind::Invalid:
    case Token::Kind::Number:
    case Token::Kind::Ident:
      s << "(" << t.val() << ")";
      break;
  }

  return s;
}

Lexer::Lexer(std::string_view expr)
    : m_expr(expr),
      m_it(m_expr.begin()),
      m_end(m_expr.end()),
      m_prev(Token::Kind::StartStream) {}

Lexer::~Lexer() {}

Token Lexer::next() {
  Token t;

  while (m_it != m_end && std::isspace(*m_it)) m_it++;
  if (m_it == m_end) return Token(Token::Kind::EndStream);

  if (std::isdigit(*m_it))
    t = _digit();
  else if (utils::isop(*m_it))
    t = _operator();
  else if (std::isalpha(*m_it))
    t = _ident();
  else
    t = Token(Token::Kind::Invalid, {(m_it++)._Unwrapped(), 1});

  m_prev = t;

  return t;
}

alias::tokens Lexer::collect() {
  std::vector<Token> tokens;
  for (Token tok = next(); tok.kind() != Token::Kind::EndStream; tok = next())
    tokens.emplace_back(tok);
  return tokens;
}

// FIXME: better number lexing
Token Lexer::_digit() {
#ifdef _UNOPTIMIZED
  std::string buf;
  while (m_it != m_end && strchr("1234567890.", *m_it) != nullptr)
    buf.push_back(*(m_it++));

  return Token(Token::Kind::Number, buf);
#else
  std::size_t n = 0;
  auto start = m_it;
  for (; m_it != m_end && strchr("1234567890.", *m_it) != nullptr; ++m_it, ++n)
    ;
  return Token(Token::Kind::Number, {start._Unwrapped(), n});
#endif
}

Token Lexer::_operator() {
  switch (*(m_it++)) {
    case '+': {
      auto& prev = m_prev;
      if (prev.kind() == Token::Kind::StartStream || prev.is_operator())
        return Token(Token::Kind::Whitespace);
      return Token(Token::Kind::PlusOp);
    }
    case '-': {
      auto& prev = m_prev;
      if (prev.kind() == Token::Kind::StartStream || prev.is_operator())
        return Token(Token::Kind::Negate);
      return Token(Token::Kind::MinusOp);
    }
    case '*':
      return Token(Token::Kind::MulOp);
    case '/':
      return Token(Token::Kind::DivideOp);
    case '%':
      return Token(Token::Kind::ModuloOp);
    case '^':
      return Token(Token::Kind::ExpOp);
    case '(':
      return Token(Token::Kind::OpenBrace);
    case ')':
      return Token(Token::Kind::CloseBrace);
    default:
      return Token(Token::Kind::Invalid);
  }
}

Token Lexer::_ident() {
#ifdef _UNOPTIMIZED
  std::string buf;
  while (m_it != m_end && std::isalnum(*m_it)) buf.push_back(*(m_it++));
  return Token(Token::Kind::Ident, buf);
#else
  std::size_t n = 0;
  auto start = m_it;
  for (; m_it != m_end && std::isalnum(*m_it); ++m_it, ++n)
    ;
  return Token(Token::Kind::Ident, {start._Unwrapped(), n});
#endif
}

void utils::tokens_dbg(const alias::tokens& tokens) {
  for (auto& tok : tokens) std::cout << tok << "\n";
}
