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
        return fibonacci(n-1) + fibonacci(n-2)

def min(a, b):
	if a < b:
		return a
	return b

def power(a, b):
	if b == 0:
		return 1
	else:
		return a * power(a, b - 1)

def calculate_circle_area(radius: float) -> float:
    pi = 3.14159
    if radius <= 0:
        return 0.0
    else:
        area = pi * radius * radius
        return area

def divide(a: float, b: float) -> float:
	if b == 0:
		return 0
	else:
		return a / b

def main():
	print(rec_add(5, 5))
	print(add(5, 5, 5))
	print(fibonacci(10))
	print(min(2, 100000))
	print(power(2, 5))
	print(calculate_circle_area(2.5))
	print(divide(10, 2))
	print(divide(10, 0))
	print(divide(10, 3))
