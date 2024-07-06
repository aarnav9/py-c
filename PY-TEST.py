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
	return a*b

def test_lists():
    my_list = [1, 2, 3]
    my_list.append(4)
    my_list.append("hi")
    print(my_list)
    print(my_list[2])
    my_list.append([1, 2, 3])
    print(my_list)
    my_list.append(my_list)
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
	print(mult("hi",3))
	test_lists()
	print(True + True)
	if -5:
		print((2 and 1))

if __name__ == '__main__':
	main()