# calc

A command-line arithmetic calculator written in C++11.

## Build

```sh
make
```

## Usage

Evaluate one or more expressions and exit:

```sh
./build/calc '1 + 2 * 3' '2^3^2' '6%4'
# 7.00
# 512.00
# 2.00
```

Run the interactive calculator with no arguments:

```sh
./build/calc
>1 + 2
3.00
>exit
Goodbye!
```

The interactive mode accepts `exit`, `exit()`, or `goodbye` (with optional whitespace) to quit. End-of-file also exits.

Supported operations: `+`, `-`, `*`, `/`, `%` (floating-point remainder), `^` (exponentiation), unary `+` and `-`, and parentheses. Decimal numbers and scientific notation are supported. Whitespace is allowed between tokens. Multiplication and division have equal precedence with `%`, and are left-associative. Exponentiation is right-associative and takes precedence over unary signs (`-2^2` is `-4`, while `(-2)^2` is `4`). Results are formatted to two decimal places.

Invalid expressions, division or remainder by zero, and non-finite results print `invalid input!`. For command-line expressions, a failure sets the process exit code to 1; the remaining arguments are still evaluated. In interactive mode, a failure prints an error and the prompt continues.

## Tests

```sh
make test
```

This runs regression tests using the Python 3 standard library (no extra dependencies).
