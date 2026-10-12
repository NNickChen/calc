#ifndef CALC_HPP
#define CALC_HPP

#include <string>

extern std::string s;      // 当前计算的算式
extern bool hasError;      // 算式是否有错误
extern int calls;          // 递归调用次数

double number(int l, int r);
int find(int l, int r);
double build(int l, int r);

#endif
