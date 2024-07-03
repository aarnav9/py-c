#include <stdio.h>

int rec_add(int a, int b) {
  if (a == 0) {
    return 0;
  }
  else {
    return (rec_add((a - 1), b) + b);
  }
}
int add(int a, int b, int v) {
  return ((a + b) + v);
}
int fibonacci(int n) {
  if (n <= 1) {
    return n;
  }
  else {
    return (fibonacci((n - 1)) + fibonacci((n - 2)));
  }
}
int min(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}
int power(int a, int b) {
  if (b == 0) {
    return 1;
  }
  else {
    return (a * power(a, (b - 1)));
  }
}
float calculate_circle_area(float radius) {
  float pi = 3.14159f;
  if (radius <= 0) {
    return 0.0f;
  }
  else {
    float area = ((float)(((float)(pi * radius)) * radius));
    return area;
  }
}
float divide(float a, float b) {
  if (b == 0) {
    return 0;
  }
  else {
    return (a / b);
  }
}
int main() {
  printf("%d\n", rec_add(5, 5));
  printf("%d\n", add(5, 5, 5));
  printf("%d\n", fibonacci(10));
  printf("%d\n", min(2, 100000));
  printf("%d\n", power(2, 5));
  printf("%f\n", calculate_circle_area(2.5f));
  printf("%f\n", divide(10, 2));
  printf("%f\n", divide(10, 0));
  printf("%f\n", divide(10, 3));
}