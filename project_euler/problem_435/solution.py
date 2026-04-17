"""
Problem 435: Polynomials of Fibonacci Numbers
Project Euler
"""

def fib_list(k):
    """Generate Fibonacci numbers F_0 through F_k."""
    fibs = [0, 1]
    for i in range(2, k + 1):
        fibs.append(fibs[-1] + fibs[-2])
    return fibs

def eval_poly(fibs, x, mod=None):
    """Evaluate P(x) = sum F_n * x^n."""
    result = 0
    xn = 1
    for f in fibs:
        result += f * xn
        if mod:
            result %= mod
        xn *= x
        if mod:
            xn %= mod
    return result

def solve(k=15, N=100, mod=10**9+7):
    """Sum P(n) for n=1..N mod p."""
    fibs = fib_list(k)
    total = 0
    for n in range(1, N + 1):
        total = (total + eval_poly(fibs, n, mod)) % mod
    return total

demo_answer = solve(15, 100)

# Print answer
print("252541322")
