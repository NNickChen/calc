#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <locale>
#include <cmath>
#include <iomanip>

//---- Parsing & Evaluation Helpers ----//

int precedence(const std::string &op) {
    if (op == "+" || op == "-")     return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "^")                  return 3;
    return 0;
}

bool isRightAssociative(const std::string &op) {
    return op == "^";
}

// Convert infix to RPN using the Shunting-Yard algorithm
std::vector<std::string> infixToRPN(const std::string &expr) {
    std::vector<std::string> output;
    std::stack<std::string> ops;
    std::istringstream in(expr);
    char c;

    while (in >> std::noskipws >> c) {
        if (std::isspace(c)) continue;

        // Hex literal (0x...)
        if (c == '0' && (in.peek()=='x' || in.peek()=='X')) {
            std::string num = "0";
            num += static_cast<char>(in.get()); // 'x' or 'X'
            while (in.peek() != EOF && std::isxdigit(in.peek())) {
                num += static_cast<char>(in.get());
            }
            output.push_back(num);
        }
        // Decimal number or decimal point
        else if (std::isdigit(c) || c == '.') {
            std::string num(1,c);
            while (in.peek() != EOF && (std::isdigit(in.peek())|| in.peek()=='.')) {
                num += static_cast<char>(in.get());
            }
            output.push_back(num);
        }
        // Function name (sin, cos, tan)
        else if (std::isalpha(c)) {
            std::string fn(1,c);
            while (in.peek()!=EOF && std::isalpha(in.peek())) {
                fn += static_cast<char>(in.get());
            }
            ops.push(fn);
        }
        // Parentheses
        else if (c == '(') {
            ops.push("(");
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top()); ops.pop();
            }
            if (ops.empty()) throw std::runtime_error("Mismatched parentheses");
            ops.pop(); // remove '('
            if (!ops.empty() && std::isalpha(ops.top()[0])) {
                output.push_back(ops.top()); ops.pop();
            }
        }
        // Operator
        else {
            std::string op(1,c);
            if (std::string("+-*/%^").find(c) != std::string::npos) {
                // Unary minus
                if (c=='-') {
                    bool unary = output.empty() || ops.empty() || ops.top()=="(" ||
                                 (precedence(ops.top())>0 && std::string("+-*/%^").find(ops.top())!=std::string::npos);
                    if (unary) {
                        std::string num("-");
                        while (in.peek()!=EOF && (std::isdigit(in.peek())||in.peek()=='.')) {
                            num += static_cast<char>(in.get());
                        }
                        output.push_back(num);
                        continue;
                    }
                }
                while (!ops.empty() && ops.top()!="(" &&
                      ((precedence(ops.top())>precedence(op)) ||
                       (precedence(ops.top())==precedence(op) && !isRightAssociative(op)))) {
                    output.push_back(ops.top()); ops.pop();
                }
                ops.push(op);
            } else {
                throw std::runtime_error(std::string("Unknown character: ")+c);
            }
        }
    }

    // Drain remaining operators
    while (!ops.empty()) {
        if (ops.top()=="("||ops.top()==")") throw std::runtime_error("Mismatched parentheses");
        output.push_back(ops.top()); ops.pop();
    }
    return output;
}

// Evaluate RPN token list
double evalRPN(const std::vector<std::string> &tokens) {
    std::stack<double> st;
    for (auto &tok : tokens) {
        if (tok=="+"||tok=="-"||tok=="*"||tok=="/"||tok=="%"||tok=="^") {
            if (st.size()<2) throw std::runtime_error("Invalid expression");
            double b=st.top(); st.pop();
            double a=st.top(); st.pop();
            if      (tok=="+") st.push(a+b);
            else if (tok=="-") st.push(a-b);
            else if (tok=="*") st.push(a*b);
            else if (tok=="/") { if (b==0) throw std::runtime_error("Division by zero"); st.push(a/b); }
            else if (tok=="%") { if (b==0) throw std::runtime_error("Modulo by zero"); st.push(std::fmod(a,b)); }
            else if (tok=="^")  st.push(std::pow(a,b));
        }
        else if (tok=="sin"||tok=="cos"||tok=="tan") {
            if (st.empty()) throw std::runtime_error("Invalid expression");
            double v=st.top(); st.pop();
            if      (tok=="sin") st.push(std::sin(v));
            else if (tok=="cos") st.push(std::cos(v));
            else if (tok=="tan") st.push(std::tan(v));
        }
        else if (tok.size()>2 && tok[0]=='0' && (tok[1]=='x'||tok[1]=='X')) {
            long long val = std::stoll(tok,nullptr,16);
            st.push(static_cast<double>(val));
        }
        else {
            st.push(std::stod(tok));
        }
    }
    if (st.size()!=1) throw std::runtime_error("Invalid expression");
    return st.top();
}

// Evaluate a string expression
double evaluate(const std::string &expr) {
    auto rpn = infixToRPN(expr);
    return evalRPN(rpn);
}

//---- Main Interactive & CLI Loop ----//

enum Base { DEC, HEX };

int main(int argc, char* argv[]) {
    Base outBase = DEC;
    try { std::locale uk("en_GB.UTF-8"); std::cout.imbue(uk); } catch(...) {}

    // CLI mode: handle --help or multiple expressions
    if (argc > 1) {
        std::string first = argv[1];
        if (first == "--help" || first == "-h") {
            std::cout << "Usage:\n"
                      << "  calc [--help] [expr1] [expr2] ...\n\n"
                      << "Evaluate each expression passed as an argument.\n"
                      << "Without expressions, starts interactive REPL.\n";
            return 0;
        }
        bool errorOccurred = false;
        for (int i = 1; i < argc; ++i) {
            std::string expr = argv[i];
            try {
                double result = evaluate(expr);
                std::cout << expr << " = " << result << '\n';
            } catch (const std::exception &e) {
                std::cerr << "Error in '" << expr << "': " << e.what() << '\n';
                errorOccurred = true;
            }
        }
        return errorOccurred ? 1 : 0;
    }

    // Interactive REPL
    std::cout << "UK CLI Calculator\n"
              << "  Supports + - * / % ^, sin(x), cos(x), tan(x), 0xHEX literals\n"
              << "  Commands: base dec, base hex, exit/quit, --help\n\n";

    std::string line;
    while (true) {
        std::cout << "calc> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit" || line == "quit") break;
        if (line == "--help" || line == "-h") {
            std::cout << "Commands:\n"
                      << "  base dec    Switch output to decimal\n"
                      << "  base hex    Switch output to hexadecimal\n"
                      << "  exit, quit  Quit the calculator\n";
            continue;
        }
        if (line.rfind("base ", 0) == 0) {
            std::string arg = line.substr(5);
            if (arg == "dec") { outBase = DEC; std::cout << "Output base: decimal\n"; }
            else if (arg == "hex") { outBase = HEX; std::cout << "Output base: hexadecimal\n"; }
            else { std::cout << "Unknown base. Use 'base dec' or 'base hex'.\n"; }
            continue;
        }
        if (line.empty()) continue;

        try {
            double result = evaluate(line);
            if (outBase == DEC) {
                std::cout << result << "\n";
            } else {
                long long iv = static_cast<long long>(result);
                std::ostringstream oss;
                oss << "0x" << std::uppercase << std::hex << iv;
                std::cout << oss.str() << std::dec << "\n";
            }
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
