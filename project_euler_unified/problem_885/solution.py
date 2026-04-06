"""
Problem 885: Modular Fibonacci Products
Compute prod_{k=1}^{n} F_k mod m using Pisano period.
"""

def fib_mod(n, m):
    """Compute F_n mod m using matrix exponentiation."""
    if n <= 0:
        return 0
    if n <= 2:
        return 1 % m
    def mat_mul(A, B, mod):
        return [[(A[0][0]*B[0][0] + A[0][1]*B[1][0]) % mod,
                 (A[0][0]*B[0][1] + A[0][1]*B[1][1]) % mod],
                [(A[1][0]*B[0][0] + A[1][1]*B[1][0]) % mod,
                 (A[1][0]*B[0][1] + A[1][1]*B[1][1]) % mod]]
    def mat_pow(M, p, mod):
        result = [[1, 0], [0, 1]]
        while p > 0:
            if p & 1:
                result = mat_mul(result, M, mod)
            M = mat_mul(M, M, mod)
            p >>= 1
        return result
    M = [[1, 1], [1, 0]]
    R = mat_pow(M, n - 1, m)
    return R[0][0]

def pisano_period(m):
    """Compute the Pisano period pi(m)."""
    if m <= 1:
        return 1
    prev, curr = 0, 1
    for i in range(1, m * m + 1):
        prev, curr = curr, (prev + curr) % m
        if prev == 0 and curr == 1:
            return i
    return -1

def fibonacci_product_mod(n, m):
    """Compute prod_{k=1}^{n} F_k mod m."""
    result = 1
    a, b = 1, 1  # F_1, F_2
    for k in range(1, n + 1):
        if k == 1:
            fk = 1
        elif k == 2:
            fk = 1
        else:
            a, b = b, (a + b)
            fk = b
        result = (result * (fk % m)) % m
    return result

def fibonacci_product_fast(n, m):
    """Compute prod using Pisano period."""
    pi = pisano_period(m)
    # Compute product over one full period
    Q = [1] * (pi + 1)
    a, b = 0, 1
    for k in range(1, pi + 1):
        a, b = b, (a + b)
        Q[k] = (Q[k-1] * (a % m)) % m
    full_periods = n // pi
    remainder = n % pi
    result = pow(Q[pi], full_periods, m) * Q[remainder] % m
    return result

# --- Verification ---
print("=== Pisano Periods ===")
for m in range(2, 21):
    pi = pisano_period(m)
    print(f"  pi({m}) = {pi}")

print("\n=== Fibonacci Product Verification ===")
for m in [7, 11, 13, 100]:
    for n in [10, 20, 50, 100]:
        slow = fibonacci_product_mod(n, m)
        fast = fibonacci_product_fast(n, m)
        match = "OK" if slow == fast else "FAIL"
        print(f"  prod F_k mod {m} (n={n}): slow={slow}, fast={fast} {match}")
        assert slow == fast

print("\n=== Fibonacci Products (small) ===")
prod = 1
for k in range(1, 12):
    fk = [0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89][k]
    prod *= fk
    print(f"  prod F_{{1..{k}}} = {prod} (mod 1000: {prod % 1000})")

answer = fibonacci_product_fast(10**6, 10**9 + 7)
print(f"\nAnswer: prod_{{k=1}}^{{10^6}} F_k mod 10^9+7 = {answer}")

# --- 4-Panel Visualization ---
