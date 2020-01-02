def fib(int, n):
    if (n == 0):
        return
    if (int <= 1):
        return int
    return (fib(int - 2, n - 1) + fib(int - 1, n - 1))

print fib (2, 4)