#!/bin/sh
# 用几个简单的算式检查修改是否正确。
set -eu

ok() {
    actual=$(./build/calc "$1")
    if [ "$actual" != "$2" ]; then
        echo "失败: $1 应得到 $2，实际得到 $actual"
        exit 1
    fi
    echo "通过: $1 = $actual"
}

bad() {
    if actual=$(./build/calc "$1"); then
        echo "失败: $1 应当报错"
        exit 1
    fi
    if [ "$actual" != "invalid input!" ]; then
        echo "失败: 错误信息不正确：$actual"
        exit 1
    fi
    echo "通过: $1 检测到错误"
}

ok '1+2*3' '7.00'
ok '(1+2)*3' '9.00'
ok '-3+5' '2.00'
ok '2^3^2' '512.00'
ok '-2^2' '-4.00'
ok '2^-2' '0.25'
ok '6%4' '2.00'
ok '1 + 2' '3.00'
ok '2*-3' '-6.00'
ok '3-2-1' '0.00'
ok '(-2)^2' '4.00'
bad '1/0'
bad '1++'
bad '(1+2'
bad '1 2'
bad '1..2'
bad '2(3)'
bad '0^-1'
echo '全部测试通过！'
