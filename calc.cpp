#include "calc.hpp"
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

string s;
bool hasError = false;
int calls = 0;

// 检查字符和括号是否正确，详细的运算格式由 build() 检查。
bool check() {
    if (s.empty()) return false;
    int brackets = 0;
    for (char c : s) {
        if (!isdigit((unsigned char)c) && c != '+' && c != '-' &&
            c != '*' && c != '/' && c != '%' && c != '^' &&
            c != '.' && c != '(' && c != ')') return false;
        if (c == '(') brackets++;
        if (c == ')') brackets--;
        if (brackets < 0 || brackets > 100) return false;
    }
    return brackets == 0;
}

// 去掉空格，避免把 "1 2" 错误地当成 12。
bool setExpression(const string& input) {
    s.clear();
    bool hadSpace = false;
    for (char c : input) {
        if (isspace((unsigned char)c)) {
            hadSpace = true;
            continue;
        }
        if (hadSpace && !s.empty() &&
            (isdigit((unsigned char)c) || c == '.') &&
            (isdigit((unsigned char)s.back()) || s.back() == '.')) {
            return false;
        }
        s += c;
        hadSpace = false;
    }
    return true;
}

bool calculate(const string& input) {
    if (!setExpression(input) || !check()) {
        cout << "invalid input!\n";
        return false;
    }
    hasError = false;
    calls = 0;
    double answer = build(0, (int)s.size() - 1);
    if (hasError || !isfinite(answer)) {
        cout << "invalid input!\n";
        return false;
    }
    cout << fixed << setprecision(2) << answer << '\n';
    return true;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        bool allOk = true;
        for (int i = 1; i < argc; i++) {
            if (!calculate(argv[i])) allOk = false;
        }
        return allOk ? 0 : 1; // 命令行计算结束后直接退出
    }

    string input;
    while (true) {
        cout << ">" << flush;
        if (!getline(cin, input)) break;
        string command;
        for (char c : input) if (!isspace((unsigned char)c)) command += c;
        if (command == "exit" || command == "exit()" || command == "goodbye") break;
        calculate(input);
    }
    cout << "Goodbye!\n";
    return 0;
}
