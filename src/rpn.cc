#include "rpn.h"

#include <exception>
#include <functional>
#include <sstream>
#include <vector>

#define APPLY_OP(STACK_NAME, EXPR) \
  STACK_NAME.push_back(            \
      _apply_operator(STACK_NAME, [](auto lhs, auto rhs) { return EXPR; }))

typedef double (*math_fn)(double);

static void _handle_operator(alias::tokens& ca, alias::tokens& tx,
                             const Token& curr);

static void _handle_close_brace(alias::tokens& ca, alias::tokens& tx);

static double _apply_operator(std::vector<double>& stack,
                              std::function<double(double, double)> cb);

static math_fn _get_math_fn_ptr(const std::string_view& name);

alias::tokens rpn::parse(const alias::tokens& tokens) {
  alias::tokens ca{}, tx{};

  for (auto& t : tokens) {
    if (t.is_number())
      ca.push_back(t);
    else if (t.is_open_brace() || t.is_ident())
      tx.push_back(t);
    else if (t.is_close_brace())
      _handle_close_brace(ca, tx);
    else if (t.is_operator())
      _handle_operator(ca, tx, t);
    else if (t.kind() == Token::Kind::Whitespace)
      continue;
    else {
      // FIXME: created separate function
      // for this
      std::stringstream ss;
      ss << "smcalc: invalid token found '" << t << "'";
      throw std::logic_error(ss.str());
    }
  }

  while (tx.size()) {
    ca.push_back(tx.back());
    tx.pop_back();
  }

  return ca;
}

double rpn::evaluate(const alias::tokens& rpn,
                     const std::map<std::string_view, double>& vars) {
  std::vector<double> stack{};

  for (auto& tok : rpn) {
    switch (tok.kind()) {
      case Token::Kind::Number:
        stack.push_back(std::atof(tok.val().data()));
        break;

      case Token::Kind::PlusOp:
        APPLY_OP(stack, lhs + rhs);
        break;

      case Token::Kind::MinusOp:
        APPLY_OP(stack, lhs - rhs);
        break;

      case Token::Kind::MulOp:
        APPLY_OP(stack, lhs * rhs);
        break;

      case Token::Kind::DivideOp:
        APPLY_OP(stack, lhs / rhs);
        break;

      case Token::Kind::ModuloOp:
        APPLY_OP(stack, std::fmod(lhs, rhs));
        break;

      case Token::Kind::ExpOp:
        APPLY_OP(stack, std::pow(lhs, rhs));
        break;

      case Token::Kind::Negate: {
        if (stack.size() < 1)
          throw std::invalid_argument(
              "smcalc: Parsing Error "
              "(Stack Underflow)");
        auto& val = stack.back();
        val = -val;
      } break;

      case Token::Kind::Ident: {
        auto fn_ptr = _get_math_fn_ptr(tok.val());

        if (fn_ptr != nullptr) {
          if (stack.size() < 1)
            throw std::invalid_argument(
                "smcalc: Parsing Error "
                "(Stack Underflow)");
          double& val = stack.back();
          val = fn_ptr(val);
        } else if (vars.contains(tok.val())) {
          stack.push_back(vars.at(tok.val()));
        }

      } break;

      default:
        std::stringstream ss;
        ss << "smcalc: invalid token "
              "found '"
           << tok << "'";
        throw std::logic_error(ss.str());
        break;
    }
  }

  return stack.back();
}

static void _handle_operator(alias::tokens& ca, alias::tokens& tx,
                             const Token& curr) {
  if (tx.size() != 0) {
    auto& tx_back = tx.back();
    if (tx_back.kind() >= curr.kind()) {
      ca.push_back(tx_back);
      tx.pop_back();
    }
  }
  tx.push_back(curr);
}

static void _handle_close_brace(alias::tokens& ca, alias::tokens& tx) {
  while (!tx.empty()) {
    if (tx.back().kind() == Token::Kind::OpenBrace) {
      tx.pop_back();
      break;
    }
    ca.push_back(tx.back());
    tx.pop_back();
  }
}

static double _apply_operator(std::vector<double>& stack,
                              std::function<double(double, double)> cb) {
  if (stack.size() < 2)
    throw std::invalid_argument(
        "smcalc: Parsing Error (Stack "
        "Underflow)");
  double rhs = stack.back();
  stack.pop_back();
  double lhs = stack.back();
  stack.pop_back();
  return cb(lhs, rhs);
}

static math_fn _get_math_fn_ptr(const std::string_view& name) {
  if (name.size() == 0) return nullptr;
  math_fn ptr = nullptr;

  if (name == "cos")
    ptr = std::cos;
  else if (name == "sin")
    ptr = std::sin;
  else if (name == "tan")
    ptr = std::tan;
  else if (name == "acos")
    ptr = std::acos;
  else if (name == "asin")
    ptr = std::asin;
  else if (name == "atan")
    ptr = std::atan;
  else if (name == "sqrt")
    ptr = std::sqrt;
  else if (name == "ln")
    ptr = std::log10;
  else if (name == "log")
    ptr = std::log;

  return ptr;
}
