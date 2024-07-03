#include <iostream>
#include <string>
#include <variant>
#include <stdexcept>
// Include test.cpp functionality
#include "test.cpp"

PY_OJ rec_add(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(a, ==, PY_OJ(0))) {
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
  if (PY_COMPARE(n, <=, PY_OJ(1))) {
    return n;
  }
  else {
    return PY_ADD(fibonacci(PY_SUB(n, PY_OJ(1))), fibonacci(PY_SUB(n, PY_OJ(2))));
  }
}
PY_OJ min(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(a, <, b)) {
    return a;
  }
  return b;
}
PY_OJ power(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(b, ==, PY_OJ(0))) {
    return PY_OJ(1);
  }
  else {
    return PY_MULT(a, power(a, PY_SUB(b, PY_OJ(1))));
  }
}
PY_OJ calculate_circle_area(PY_OJ radius) {
  PY_OJ pi = PY_OJ(3.14159f);
  if (PY_COMPARE(radius, <=, PY_OJ(0))) {
    return PY_OJ(0.0f);
  }
  else {
    PY_OJ area = PY_MULT(PY_MULT(pi, radius), radius);
    return area;
  }
}
PY_OJ divide(PY_OJ a, PY_OJ b) {
  if (PY_COMPARE(b, ==, PY_OJ(0))) {
    return PY_OJ(0);
  }
  else {
    return PY_DIV(a, b);
  }
}
void py_main() {
  PY_PRINT(rec_add(PY_OJ(5), PY_OJ(5)));
  PY_PRINT(add(PY_OJ(5), PY_OJ(5), PY_OJ(5)));
  PY_PRINT(fibonacci(PY_OJ(10)));
  PY_PRINT(min(PY_OJ(2), PY_OJ(100000)));
  PY_PRINT(power(PY_OJ(2), PY_OJ(5)));
  PY_PRINT(calculate_circle_area(PY_OJ(2.5f)));
  PY_PRINT(divide(PY_OJ(10), PY_OJ(2)));
  PY_PRINT(divide(PY_OJ(10), PY_OJ(0)));
  PY_PRINT(divide(PY_OJ(10), PY_OJ(3)));
}
int main() {
    py_main();
    return 0;
}