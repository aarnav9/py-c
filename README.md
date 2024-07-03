# Layout
input.py: Python code

python-c.py: Lexer, Parser, Transpiler

test-oj.cpp: Python functionality and typing in C++

output.cpp: Translated code

# Commands
run python3 python-c.py

clang++ -std=c++17 output.cpp -o test && ./test
