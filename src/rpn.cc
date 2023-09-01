#include "rpn.h"

#include <exception>
#include <sstream>

static void _handle_operator(alias::tokens& ca, alias::tokens& tx,
                             const Token& curr);

static void _handle_close_brace(alias::tokens& ca, alias::tokens& tx);

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
    else {
      // FIXME: created separate function for this
      std::stringstream ss;
      ss << "smcalc: invalid token found '" << t.val() << "'";
      throw std::logic_error(ss.str());
    }
  }

  while (tx.size()) {
    ca.push_back(tx.back());
    tx.pop_back();
  }

  return ca;
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
