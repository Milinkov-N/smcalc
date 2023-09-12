#include <gtest/gtest.h>

#include <exception>

#include "calc.h"

TEST(Calculator, JustANumber) {
  Calculator calc("1");
  ASSERT_EQ(calc.eval(), 1);
}

TEST(Calculator, OnePlusOne) {
  Calculator calc("1+1");
  ASSERT_EQ(calc.eval(), 2);
}

TEST(Calculator, MinusOnePlusOne) {
  Calculator calc("-1+1");
  ASSERT_EQ(calc.eval(), 0);
}

TEST(Calculator, OnePlusMinusOne) {
  Calculator calc("1+-1");
  ASSERT_EQ(calc.eval(), 0);
}

TEST(Calculator, TenMinusEight) {
  Calculator calc("10-8");
  ASSERT_EQ(calc.eval(), 2);
}

TEST(Calculator, TenTimesEight) {
  Calculator calc("10*8");
  ASSERT_EQ(calc.eval(), 80);
}

TEST(Calculator, TenDivideEight) {
  Calculator calc("10/8");
  ASSERT_EQ(calc.eval(), 1.25);
}

TEST(Calculator, TenModuloFive) {
  Calculator calc("10%5");
  ASSERT_EQ(calc.eval(), 0);
}

TEST(Calculator, TenExpTwo) {
  Calculator calc("10^2");
  ASSERT_EQ(calc.eval(), 100);
}

TEST(Calculator, Float) {
  Calculator calc("1.2345");
  ASSERT_EQ(calc.eval(), 1.2345);
}

TEST(Calculator, InvalidFloat) {
  Calculator calc("1.2345.3");
  EXPECT_THROW(calc.eval(), std::logic_error);
}

TEST(Calculator, FnSin) {
  Calculator calc("sin(3.14)");
  ASSERT_EQ(calc.eval(), sin(3.14));
}

TEST(Calculator, FnCos) {
  Calculator calc("cos(3.14)");
  ASSERT_EQ(calc.eval(), cos(3.14));
}

TEST(Calculator, FnTan) {
  Calculator calc("tan(3.14)");
  ASSERT_EQ(calc.eval(), tan(3.14));
}

TEST(Calculator, FnAsin) {
  Calculator calc("asin(0.25)");
  ASSERT_EQ(calc.eval(), asin(0.25));
}

TEST(Calculator, FnAcos) {
  Calculator calc("acos(0.25)");
  ASSERT_EQ(calc.eval(), acos(0.25));
}

TEST(Calculator, FnAtan) {
  Calculator calc("atan(3.14)");
  ASSERT_EQ(calc.eval(), atan(3.14));
}

TEST(Calculator, FnSqrt) {
  Calculator calc("sqrt(3.14)");
  ASSERT_EQ(calc.eval(), sqrt(3.14));
}

TEST(Calculator, FnLn) {
  Calculator calc("ln(3.14)");
  ASSERT_EQ(calc.eval(), log10(3.14));
}

TEST(Calculator, FnLog) {
  Calculator calc("log(3.14)");
  ASSERT_EQ(calc.eval(), log(3.14));
}

TEST(Calculator, Variable) {
  Calculator calc("x", {{"x", 3.14}});
  ASSERT_EQ(calc.eval(), 3.14);
}

TEST(Calculator, CosOnePlusTwo) {
    Calculator calc("cos(1) + 2");

    double expected = cos(1) + 2;
    ASSERT_EQ(calc.eval(), expected);
}

TEST(Calculator, ComplexExpression01) {
  double x = 0.25, y = 10, PI = 3.14;
  Calculator calc("(cos(PI) ^ y + tan(x))", {{"x", x}, {"y", y}, {"PI", PI}});

  double expected = (pow(cos(PI), y) + tan(x));
  ASSERT_EQ(calc.eval(), expected);
}

TEST(Calculator, ComplexExpression02) {
  double x = 0.25, y = 10, PI = 3.14;
  Calculator calc("-(-cos(PI) ^ y + tan(x))", {{"x", x}, {"y", y}, {"PI", PI}});

  double expected = -(pow(-cos(PI), y) + tan(x));
  ASSERT_EQ(calc.eval(), expected);
}

TEST(Calculator, ComplexExpressionXX) {
  double x = 0.25, y = 10, PI = 3.14;
  Calculator calc("sin(x * 12.5) - (cos(PI) ^ y + tan(x))",
                  {{"x", x}, {"y", y}, {"PI", PI}});

  double expected = sin(x * 12.5) - (pow(cos(PI), y) + tan(x));
  ASSERT_EQ(calc.eval(), expected);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
