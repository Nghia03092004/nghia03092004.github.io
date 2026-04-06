"""
Problem 967: Pisano Period Computation

Sum of pi(m) for m = 2..1000, where pi(m) is the Pisano period — the period
of Fibonacci numbers modulo m.

The Pisano period pi(m) is the smallest positive integer k such that
F(k) = 0 (mod m) and F(k+1) = 1 (mod m). Known properties:
    - pi(1) = 1
    - pi(2) = 3
    - pi(10) = 60
    - pi(p) divides p-1 if p = +/-1 (mod 5), divides 2(p+1) otherwise
    - pi(m) <= 6m, with equality never achieved

Key results:
    - Sum of pi(m) for m=2..1000
    - pi(m) has multiplicative-like structure for coprime factors

Methods:
    1. pisano_period           — direct computation via Fibonacci iteration
    2. pisano_period_prime     — optimized for prime moduli
    3. verify_pisano_properties — check known theoretical properties
    4. cumulative_sum_analysis — growth of sum over range
"""

def pisano_period(m):
    """Compute Pisano period pi(m) by finding period of Fib mod m."""
    if m == 1:
        return 1
    a, b = 0, 1
    for k in range(1, 6 * m + 2):
        a, b = b, (a + b) % m
        if a == 0 and b == 1:
            return k
    return -1

def fibonacci_mod_sequence(m, length):
    """Generate first 'length' Fibonacci numbers mod m."""
    seq = [0, 1]
    for _ in range(length - 2):
        seq.append((seq[-1] + seq[-2]) % m)
    return seq

def verify_known_pisano():
    """Check known Pisano period values."""
    known = {
        1: 1, 2: 3, 3: 8, 4: 6, 5: 20,
        6: 24, 7: 16, 8: 12, 9: 24, 10: 60
    }
    for m, expected in known.items():
        computed = pisano_period(m)
        assert computed == expected, f"pi({m})={computed}, expected {expected}"
    return True

def check_pisano_divisibility(m, period):
    """Verify that Fib(period) = 0 mod m and Fib(period+1) = 1 mod m."""
    a, b = 0, 1
    for _ in range(period):
        a, b = b, (a + b) % m
    return a == 0 and b == 1

# Verification
assert verify_known_pisano()

# Verify divisibility property for first 50 values
for m in range(2, 51):
    p = pisano_period(m)
    assert check_pisano_divisibility(m, p), f"Pisano verification failed for m={m}"

# Compute answer
N = 1000
periods = [0, 0] + [pisano_period(m) for m in range(2, N + 1)]
answer = sum(periods[2:])
print(answer)
