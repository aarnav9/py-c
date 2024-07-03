Layout

input.py: Python code

python-c.py: Lexer, Parser, Transpiler

test-oj.cpp: Python functionality and typing in C++

output.cpp: Translated code

Commands

python3 python-c.py

clang++ -std=c++17 output.cpp -o test && ./test

Functionality

Currently only supports types int, float, char, string with operations +, -, *, /, if, else. This means any functions running these will work including recursive calls and powerful nested functions.
