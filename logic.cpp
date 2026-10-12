#include "calc.hpp"
#include <cmath>
#include <cctype>

using namespace std;

// 将 s[l..r] 中的数字变成 double。
double number(int l, int r) {
    if (l > r) { hasError = true; return 0; }
    double value = 0;
    double place = 0.1;
    bool dot = false, digit = false;
    for (int i = l; i <= r; i++) {
        if (s[i] == '.') {
            if (dot) { hasError = true; return 0; }
            dot = true;
        } else if (isdigit((unsigned char)s[i])) {
            digit = true;
            if (!dot) value = value * 10 + (s[i] - '0');
            else { value += (s[i] - '0') * place; place /= 10; }
        } else {
            hasError = true;
            return 0;
        }
    }
    if (!digit || !isfinite(value)) hasError = true;
    return value;
}

// 找到当前括号外优先级最低的运算符。
// 加减、乘除取模选最右边（从左到右算），乘方选最左边（从右到左算）。
int find(int l, int r) {
    int brackets = 0, plusMinus = -1, multiply = -1, power = -1;
    for (int i = l; i <= r; i++) {
        char c = s[i];
        if (c == '(') { brackets++; continue; }
        if (c == ')') { brackets--; continue; }
        if (brackets != 0) continue;
        if (c == '+' || c == '-') {
            // 开头或运算符后面的 +、- 是正负号，不是加减法。
            if (i == l || s[i-1] == '+' || s[i-1] == '-' ||
                s[i-1] == '*' || s[i-1] == '/' || s[i-1] == '%' ||
                s[i-1] == '^' || s[i-1] == '(') continue;
            plusMinus = i;
        } else if (c == '*' || c == '/' || c == '%') {
            multiply = i;
        } else if (c == '^' && power == -1) {
            power = i;
        }
    }
    if (plusMinus != -1) return plusMinus;
    if (multiply != -1) return multiply;
    return power;
}

// 递归计算 s[l..r]。
double build(int l, int r) {
    // 限制过长算式，防止过多递归导致程序崩溃。
    if (++calls > 1000 || l > r) { hasError = true; return 0; }

    int p = find(l, r);
    if (p != -1 && s[p] != '^') {
        double a = build(l, p - 1);
        double b = build(p + 1, r);
        if (s[p] == '+') return a + b;
        if (s[p] == '-') return a - b;
        if (s[p] == '*') return a * b;
        if (b == 0) { hasError = true; return 0; }
        if (s[p] == '/') return a / b;
        return fmod(a, b); // %：取余数
    }

    // 负号的优先级低于乘方：-2^2 = -(2^2)
    if (s[l] == '-') return -build(l + 1, r);
    if (s[l] == '+') return build(l + 1, r);
    if (p != -1) return pow(build(l, p - 1), build(p + 1, r));

    // 没有运算符时，可能是括号，也可能是一个数字。
    if (s[l] == '(' && s[r] == ')') return build(l + 1, r - 1);
    return number(l, r);
}
