#!/usr/bin/env python3
"""Run regression tests against the built calculator binary."""

import subprocess
import unittest
from pathlib import Path

BINARY = Path(__file__).resolve().parents[1] / "build" / "calc"


def run_calc(*args, input_text=None):
    return subprocess.run(
        [str(BINARY), *args],
        input=input_text,
        text=True,
        capture_output=True,
        timeout=5,
        check=False,
    )


class CalculatorTests(unittest.TestCase):
    def test_precedence_associativity_unary_and_decimals(self):
        expressions = [
            "1+2*3", "(1+2)*3", "8/4/2", "1-2-3", "6%4", "5.5%2",
            "2^3^2", "(-2)^3", "-2^2", "2^-2", "-3+5", "+3", "--3",
            "1e2+5", ".5+1", "1.+2", "2*-3", "2--3", "(-2)^2",
        ]
        expected = [
            "7.00", "9.00", "1.00", "-4.00", "2.00", "1.50",
            "512.00", "-8.00", "-4.00", "0.25", "2.00", "3.00", "3.00",
            "105.00", "1.50", "3.00", "-6.00", "5.00", "4.00",
        ]
        result = run_calc(*expressions)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout.splitlines(), expected)

    def test_rejects_malformed_expressions_and_invalid_arithmetic(self):
        expressions = [
            "", ".", "1..2", "1+", "1++", "(1+2", "1+2)", "()",
            "2(3+4)", "1/0", "1%0", "(-2)^0.5", "1e999", "2^^3",
            "abc", "1e", "5//2", "1_2", "1 2", "0^-1", "1e+",
        ]
        result = run_calc(*expressions)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(result.stdout.splitlines(), ["invalid input!"] * len(expressions))

    def test_interactive_mode_supports_spaces_and_exit(self):
        result = run_calc(input_text=" 2 + 3 * 4 \n1/0\n exit() \n")
        self.assertEqual(result.returncode, 0)
        self.assertEqual(result.stdout, ">14.00\n>invalid input!\n>Goodbye!\n")

    def test_interactive_mode_handles_eof(self):
        result = run_calc(input_text="1+1\n")
        self.assertEqual(result.returncode, 0)
        self.assertEqual(result.stdout, ">2.00\n>Goodbye!\n")

    def test_no_fixed_buffer_limit(self):
        result = run_calc(input_text=" " * 1200 + "5\nexit\n")
        self.assertEqual(result.returncode, 0)
        self.assertEqual(result.stdout, ">5.00\n>Goodbye!\n")

    def test_deep_nesting_is_rejected_without_crashing(self):
        expressions = ["(" * 2000 + "1" + ")" * 2000, "-" * 2000 + "1"]
        result = run_calc(*expressions)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertEqual(result.stdout.splitlines(), ["invalid input!"] * 2)

    def test_nonfinite_intermediate_result_is_rejected(self):
        result = run_calc("1e308*1e308", "1e308+1e308")
        self.assertEqual(result.returncode, 1)
        self.assertEqual(result.stdout.splitlines(), ["invalid input!"] * 2)


if __name__ == "__main__":
    unittest.main()
