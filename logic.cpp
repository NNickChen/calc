#include "calc.hpp"

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace {

class Parser {
public:
    explicit Parser(const std::string& expression)
        : expression_(expression), position_(0), nestingDepth_(0) {}

    double parse() {
        const double result = expression();
        skipWhitespace();
        if (position_ != expression_.size()) {
            throw std::invalid_argument("unexpected characters");
        }
        return result == 0.0 ? 0.0 : result;
    }

private:
    const std::string& expression_;
    std::size_t position_;
    std::size_t nestingDepth_;

    // Bound recursive input to avoid a stack overflow on deeply nested expressions.
    class NestingGuard {
    public:
        explicit NestingGuard(std::size_t& depth) : depth_(depth) {
            if (depth_ >= 256) {
                throw std::invalid_argument("expression nested too deeply");
            }
            ++depth_;
        }
        ~NestingGuard() { --depth_; }

    private:
        std::size_t& depth_;
    };

    void skipWhitespace() {
        while (position_ < expression_.size() &&
               std::isspace(static_cast<unsigned char>(expression_[position_]))) {
            ++position_;
        }
    }

    bool consume(char symbol) {
        skipWhitespace();
        if (position_ < expression_.size() && expression_[position_] == symbol) {
            ++position_;
            return true;
        }
        return false;
    }

    static double finite(double value) {
        if (!std::isfinite(value)) {
            throw std::invalid_argument("result is not finite");
        }
        return value;
    }

    double expression() {
        NestingGuard nesting(nestingDepth_);
        double value = term();
        while (true) {
            if (consume('+')) {
                value = finite(value + term());
            } else if (consume('-')) {
                value = finite(value - term());
            } else {
                return value;
            }
        }
    }

    double term() {
        double value = unary();
        while (true) {
            if (consume('*')) {
                value = finite(value * unary());
            } else if (consume('/')) {
                const double divisor = unary();
                if (divisor == 0.0) {
                    throw std::invalid_argument("division by zero");
                }
                value = finite(value / divisor);
            } else if (consume('%')) {
                const double divisor = unary();
                if (divisor == 0.0) {
                    throw std::invalid_argument("modulo by zero");
                }
                value = finite(std::fmod(value, divisor));
            } else {
                return value;
            }
        }
    }

    double unary() {
        NestingGuard nesting(nestingDepth_);
        if (consume('+')) {
            return unary();
        }
        if (consume('-')) {
            return finite(-unary());
        }
        return power();
    }

    double power() {
        const double base = primary();
        // Parsing the exponent as unary permits 2^-3 and makes ^ right-associative.
        if (consume('^')) {
            return finite(std::pow(base, unary()));
        }
        return base;
    }

    double primary() {
        if (consume('(')) {
            const double value = expression();
            if (!consume(')')) {
                throw std::invalid_argument("missing closing parenthesis");
            }
            return value;
        }
        return number();
    }

    double number() {
        skipWhitespace();
        const std::size_t start = position_;
        bool hasDigit = false;
        while (position_ < expression_.size() &&
               std::isdigit(static_cast<unsigned char>(expression_[position_]))) {
            hasDigit = true;
            ++position_;
        }
        if (position_ < expression_.size() && expression_[position_] == '.') {
            ++position_;
            while (position_ < expression_.size() &&
                   std::isdigit(static_cast<unsigned char>(expression_[position_]))) {
                hasDigit = true;
                ++position_;
            }
        }
        if (!hasDigit) {
            throw std::invalid_argument("number expected");
        }
        if (position_ < expression_.size() &&
            (expression_[position_] == 'e' || expression_[position_] == 'E')) {
            ++position_;
            if (position_ < expression_.size() &&
                (expression_[position_] == '+' || expression_[position_] == '-')) {
                ++position_;
            }
            const std::size_t exponentStart = position_;
            while (position_ < expression_.size() &&
                   std::isdigit(static_cast<unsigned char>(expression_[position_]))) {
                ++position_;
            }
            if (position_ == exponentStart) {
                throw std::invalid_argument("invalid exponent");
            }
        }
        const std::string token = expression_.substr(start, position_ - start);
        char* end = nullptr;
        const double value = std::strtod(token.c_str(), &end);
        if (end != token.c_str() + token.size()) {
            throw std::invalid_argument("invalid number");
        }
        return finite(value);
    }
};

} // namespace

double evaluate(const std::string& expression) {
    return Parser(expression).parse();
}
