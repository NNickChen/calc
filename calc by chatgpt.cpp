#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <locale>

// 获取运算符优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 将中缀表达式转换为后缀（逆波兰）表达式
std::vector<std::string> infixToRPN(const std::string &expr) {
    std::vector<std::string> output;
    std::stack<char> ops;
    std::istringstream in(expr);
    char c;

    while (in >> std::noskipws >> c) {
        if (std::isspace(c)) {
            continue;
        }
        // 数字或小数点
        if (std::isdigit(c) || c == '.') {
            std::string num(1, c);
            // 读取完整数字
            while (in.peek() != EOF && (std::isdigit(in.peek()) || in.peek() == '.')) {
                num += static_cast<char>(in.get());
            }
            output.push_back(num);
        }
        // 左括号
        else if (c == '(') {
            ops.push(c);
        }
        // 右括号
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output.push_back(std::string(1, ops.top()));
                ops.pop();
            }
            if (ops.empty()) throw std::runtime_error("括号不匹配");
            ops.pop(); // 弹出左括号
        }
        // 操作符
        else if (c=='+' || c=='-' || c=='*' || c=='/') {
            // 处理一元负号：如果当前是 '-' 且前一输出是操作符或栈空，视为负号的一部分
            if (c == '-') {
                bool unary = output.empty() ||
                             (!output.empty() && output.back().size() == 1 && std::string("+-*/").find(output.back()) != std::string::npos);
                if (unary) {
                    // 把负号当作数字开头
                    std::string num("-");
                    while (in.peek() != EOF && (std::isdigit(in.peek()) || in.peek() == '.')) {
                        num += static_cast<char>(in.get());
                    }
                    output.push_back(num);
                    continue;
                }
            }
            // 普通二元运算符
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)) {
                output.push_back(std::string(1, ops.top()));
                ops.pop();
            }
            ops.push(c);
        }
        else {
            throw std::runtime_error(std::string("未知字符：") + c);
        }
    }

    // 清空剩余运算符
    while (!ops.empty()) {
        if (ops.top() == '(' || ops.top() == ')') throw std::runtime_error("括号不匹配");
        output.push_back(std::string(1, ops.top()));
        ops.pop();
    }
    return output;
}

// 计算逆波兰表达式
double evalRPN(const std::vector<std::string> &tokens) {
    std::stack<double> st;
    for (const auto &tok : tokens) {
        if (tok == "+" || tok == "-" || tok == "*" || tok == "/") {
            if (st.size() < 2) throw std::runtime_error("表达式错误");
            double b = st.top(); st.pop();
            double a = st.top(); st.pop();
            if (tok == "+") st.push(a + b);
            else if (tok == "-") st.push(a - b);
            else if (tok == "*") st.push(a * b);
            else if (tok == "/") {
                if (b == 0) throw std::runtime_error("除以零错误");
                st.push(a / b);
            }
        } else {
            st.push(std::stod(tok));
        }
    }
    if (st.size() != 1) throw std::runtime_error("表达式错误");
    return st.top();
}

// 对外接口：计算字符串表达式
double evaluate(const std::string &expr) {
    auto rpn = infixToRPN(expr);
    return evalRPN(rpn);
}

int main() {
    // 设置英国本地化输出（千位以逗号分组，小数点为 .）
    try {
        std::locale gb("en_GB.UTF-8");
        std::cout.imbue(gb);
    } catch (...) {
        // 环境不支持时忽略
    }

    std::string line;
    std::cout << "英国 CLI 计算器 (输入 exit 或 quit 退出)\n";
    while (true) {
        std::cout << "calc> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit" || line == "quit") break;
        if (line.empty()) continue;
        try {
            double result = evaluate(line);
            std::cout << result << "\n";
        } catch (const std::exception &e) {
            std::cout << "错误: " << e.what() << "\n";
        }
    }
    return 0;
}
