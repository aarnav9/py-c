### Layout

input.py: Python code

python-c.py: Lexer, Parser, Transpiler

test-oj.cpp: Python functionality and typing in C++

output.cpp: Translated code

### More Funtionality but unstable

PY2-CPP.py: Lexer, Parser, Transpiler

PY2.cpp: Python functionality and typing in C++

### Commands

python3 python-c.py

clang++ -std=c++17 output.cpp -o test && ./test

### Functionality

Currently only supports types int, float, char, string, list, with operations +, -, *, /, <, <=, ==, >=, >, if, else, append. This means any functions running these will work including recursive calls and powerful nested functions.

### Examples
```Python
def rec_add(a, b):
    if a == 0:
        return 0
    else:
        return rec_add((a - 1), b) + b


def add(a, b, v):
    return a + b + v


def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)


def min(a, b):
    if a < b:
        return a
    return b


def power(a, b):
    if b == 0:
        return 1
    else:
        return a * power(a, b - 1)


def calculate_circle_area(radius):
    pi = 3.14159
    if radius <= 0:
        return 0.0
    else:
        area = pi * radius * radius
        return area


def divide(a, b):
    if b == 0:
        return 0
    else:
        return a / b


def nested(a, b):
    if a < b:
        if a == 0:
            return 0
        else:
            return min(a, 3)
    else:
        if b == 0:
            return 0
        else:
            return b


def abs(n):
    if n < 0:
        return n * 1
    else:
        return n


def fib_next(n):
    if n < 0:
        return 0
    else:
        return "hello"


def mult(a, b):
    return a * b


def test_lists():
    my_list = [1, 2, 3]
    my_list.append(4)
    my_list.append("hi")
    print(my_list)
    print(my_list[2])
    my_list.append([1, 2, 3])
    print(my_list)


def main():
    print(rec_add(5, 5))
    print(add(5, 5, 5))
    print(fibonacci(10))
    print(fibonacci(20))
    print(min(2, 100000))
    print(power(2, 5))
    print(calculate_circle_area(2.5))
    print(divide(10, 2))
    print(divide(10, 0))
    print(divide(10, 3))
    print(nested(5, 10))
    print(fib_next(10))
    print(mult("hi", 3))
    test_lists()
```

The output C++ code for the function above

```C++
#include <iostream>
#include <string>
#include <variant>
#include <stdexcept>
// Include PY.cpp functionality
#include "PY2.cpp"

PY_OJ rec_add(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(a, std::equal_to<>(), PY_OJ(0))) {
    return PY_OJ(0);
  }
  else {
    return PY_ADD(rec_add(PY_SUB(a, PY_OJ(1)), b), b);
  }
}
PY_OJ add(PY_OJ a, PY_OJ b, PY_OJ v) {
  return PY_ADD(PY_ADD(a, b), v);
}
PY_OJ fibonacci(PY_OJ n) {
  if (PY_COMPARE(n, std::less_equal<>(), PY_OJ(1))) {
    return n;
  }
  else {
    return PY_ADD(fibonacci(PY_SUB(n, PY_OJ(1))), fibonacci(PY_SUB(n, PY_OJ(2))));
  }
}
PY_OJ min(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(a, std::less<>(), b)) {
    return a;
  }
  return b;
}
PY_OJ power(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(b, std::equal_to<>(), PY_OJ(0))) {
    return PY_OJ(1);
  }
  else {
    return PY_MULT(a, power(a, PY_SUB(b, PY_OJ(1))));
  }
}
PY_OJ calculate_circle_area(PY_OJ radius) {
  auto pi = PY_OJ(3.14159f);
  if (PY_COMPARE(radius, std::less_equal<>(), PY_OJ(0))) {
    return PY_OJ(0.0f);
  }
  else {
    auto area = PY_MULT(PY_MULT(pi, radius), radius);
    return area;
  }
}
PY_OJ divide(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(b, std::equal_to<>(), PY_OJ(0))) {
    return PY_OJ(0);
  }
  else {
    return PY_DIV(a, b);
  }
}
PY_OJ nested(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(a, std::less<>(), b)) {
    if (PY_COMPARE(a, std::equal_to<>(), PY_OJ(0))) {
      return PY_OJ(0);
    }
    else {
      return min(a, PY_OJ(3));
    }
  }
  else {
    if (PY_COMPARE(b, std::equal_to<>(), PY_OJ(0))) {
      return PY_OJ(0);
    }
    else {
      return b;
    }
  }
}
PY_OJ abs(PY_OJ n) {
  if (PY_COMPARE(n, std::less<>(), PY_OJ(0))) {
    return PY_MULT(n, PY_OJ(1));
  }
  else {
    return n;
  }
}
PY_OJ fib_next(PY_OJ n) {
  if (PY_COMPARE(n, std::less<>(), PY_OJ(0))) {
    return PY_OJ(0);
  }
  else {
    return PY_OJ("hello");
  }
}
PY_OJ mult(PY_OJ a, PY_OJ b) {
  return PY_MULT(a, b);
}
void test_lists() {
  auto my_list = PY_OJ({std::vector<PY_OJ>{PY_OJ(1), PY_OJ(2), PY_OJ(3)}});
  PY_LIST_APPEND(my_list, PY_OJ(4));
  PY_LIST_APPEND(my_list, PY_OJ("hi"));
  PY_PRINT(my_list);
  PY_PRINT(PY_LIST_GET(my_list, PY_OJ(2)));
  PY_LIST_APPEND(my_list, PY_OJ({std::vector<PY_OJ>{PY_OJ(1), PY_OJ(2), PY_OJ(3)}}));
  PY_PRINT(my_list);
}
void py_main() {
  PY_PRINT(rec_add(PY_OJ(5), PY_OJ(5)));
  PY_PRINT(add(PY_OJ(5), PY_OJ(5), PY_OJ(5)));
  PY_PRINT(fibonacci(PY_OJ(10)));
  PY_PRINT(fibonacci(PY_OJ(20)));
  PY_PRINT(min(PY_OJ(2), PY_OJ(100000)));
  PY_PRINT(power(PY_OJ(2), PY_OJ(5)));
  PY_PRINT(calculate_circle_area(PY_OJ(2.5f)));
  PY_PRINT(divide(PY_OJ(10), PY_OJ(2)));
  PY_PRINT(divide(PY_OJ(10), PY_OJ(0)));
  PY_PRINT(divide(PY_OJ(10), PY_OJ(3)));
  PY_PRINT(nested(PY_OJ(5), PY_OJ(10)));
  PY_PRINT(fib_next(PY_OJ(10)));
  PY_PRINT(mult(PY_OJ("hi"), PY_OJ(3)));
  test_lists();
}
int main() {
    py_main();
    return 0;
}
```
