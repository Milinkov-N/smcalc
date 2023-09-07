#include <gtest/gtest.h>

#include "lexer.h"
#include "rpn.h"

TEST(LexerTest, Basic) {
  Lexer lexer("1 + 2 + 3");
  auto tokens = lexer.collect();
  auto it = tokens.begin();

  Token expected_tokens[] = {{Token::Kind::Number, "1"},
                             {Token::Kind::PlusOp},
                             {Token::Kind::Number, "2"},
                             {Token::Kind::PlusOp},
                             {Token::Kind::Number, "3"}};

  for (int i = 0; it != tokens.end(); ++it, ++i)
    ASSERT_EQ(*it, expected_tokens[i]);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
