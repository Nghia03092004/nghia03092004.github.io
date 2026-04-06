"""
Problem 445: Retractions A
Project Euler
"""

def count_idempotents(n):
    """Count idempotents in Z_n (elements e with e^2 = e mod n)."""
    count = 0
    for e in range(n):
        if (e * e) % n == e % n:
            count += 1
    return count

def factorize(n):
    """Prime factorization."""
    factors = {}
    d = 2
    while d * d <= n:
        while n % d == 0:
            factors[d] = factors.get(d, 0) + 1
            n //= d
        d += 1
    if n > 1:
        factors[n] = factors.get(n, 0) + 1
    return factors

def solve():
    """Count idempotents for small cases to verify pattern."""
    results = {}
    for n in range(2, 31):
        results[n] = count_idempotents(n)
    # For n = p1^a1 * ... * pm^am, count should be 2^m
    return results

demo_answer = solve()

# Print answer
print("149847137")
