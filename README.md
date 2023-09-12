# SmartCalc

Small and simple to use math expression evaluator with command line interface written in C++.

This application uses Reverse Polish Notation (RPN for short) for evaluating mathematical expressions and have all basic functions, like `cos()` or `sqrt()` and also supports variables.

It is a hobby project, so some bugs can occur, but in very rare edge cases. Oh, and don't use function names as variables: they are like a *keywords* and this can cause some unvanted behavior (most of the time it's will just print an error).

## Example usage

```bash
smcalc "cos(x) * y" --var x:10 --var y:2

# Alternate syntax that evaluates to the same result
smcalc "cos x * y" --var x:10 --var y:2

# Not valid
smcalc "cos + 2" --var cos:2
```