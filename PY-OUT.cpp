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
  PY_LIST_APPEND(my_list, my_list);
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