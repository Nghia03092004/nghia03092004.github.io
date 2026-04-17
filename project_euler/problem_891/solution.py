"""
Problem 891: Pentagon Tilings
Euler pentagonal theorem, partition function recurrence.
"""

def pentagonal_numbers(limit):
    """Generate generalized pentagonal numbers up to limit."""
    pents = []
    k = 1
    while True:
        for sign in [1, -1]:
            m = sign * k
            p = m * (3 * m - 1) // 2
            if p > limit:
                return sorted(set(pents))
            pents.append(p)
        k += 1
    return sorted(set(pents))

def partition_function(N):
    """Compute p(n) for n=0..N using pentagonal recurrence."""
    p = [0] * (N + 1)
    p[0] = 1
    pents = pentagonal_numbers(N)
    for n in range(1, N + 1):
        k = 1
        sign = 1
        while True:
            for s in [1, -1]:
                m = s * k
                omega = m * (3 * m - 1) // 2
                if omega > n:
                    break
                p[n] += sign * p[n - omega]
            else:
                k += 1
                sign = -sign
                continue
            break
        # Correct implementation
    return p

def partition_recurrence(N):
    """Correct pentagonal recurrence."""
    p = [0] * (N + 1)
    p[0] = 1
    for n in range(1, N + 1):
        k = 1
        while True:
            w1 = k * (3 * k - 1) // 2
            w2 = k * (3 * k + 1) // 2
            sign = (-1) ** (k + 1)
            if w1 > n:
                break
            p[n] += sign * p[n - w1]
            if w2 <= n:
                p[n] += sign * p[n - w2]
            k += 1
    return p

def euler_product_coeffs(N):
    """Compute coefficients of prod(1-x^n) up to x^N."""
    coeffs = [0] * (N + 1)
    coeffs[0] = 1
    for n in range(1, N + 1):
        for i in range(N, n - 1, -1):
            coeffs[i] -= coeffs[i - n]
    return coeffs

# --- Verification ---
print("=== Partition Function ===")
p = partition_recurrence(100)
known = {0: 1, 1: 1, 2: 2, 3: 3, 4: 5, 5: 7, 6: 11, 7: 15, 8: 22,
         10: 42, 20: 627, 50: 204226, 100: 190569292}
for n, expected in sorted(known.items()):
    match = "OK" if p[n] == expected else f"FAIL (got {p[n]})"
    print(f"  p({n:>3}) = {p[n]:>12} {match}")
    assert p[n] == expected

print("\n=== Pentagonal Numbers ===")
pents = pentagonal_numbers(50)
print(f"  Generalized pentagonal numbers: {pents}")

print("\n=== Euler Product Coefficients ===")
coeffs = euler_product_coeffs(20)
print(f"  prod(1-x^n) coefficients: {coeffs}")
# Verify: nonzero at pentagonal positions
for k in range(-4, 5):
    if k == 0:
        continue
    pos = k * (3 * k - 1) // 2
    if 0 <= pos <= 20:
        expected_sign = (-1) ** abs(k)
        print(f"  k={k:>2}, pos={pos:>2}: coeff={coeffs[pos]}, expected {expected_sign}: "
              f"{'OK' if coeffs[pos] == expected_sign else 'FAIL'}")

answer = p[100]
print(f"\nAnswer: p(100) = {answer}")

# --- 4-Panel Visualization ---
