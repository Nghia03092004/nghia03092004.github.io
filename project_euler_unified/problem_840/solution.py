"""
Problem 840: Sum of Products

Compute sum of all elementary symmetric polynomials e_1 + e_2 + ... + e_n.
Key insight: sum = prod(1 + x_i) - 1.
"""

from math import comb

MOD = 10**9 + 7

# --- Method 1: Product formula (optimal) ---
def sum_esp_product(xs, mod=None):
    """sum_{k=1}^{n} e_k = prod(1 + x_i) - 1."""
    result = 1
    for x in xs:
        if mod:
            result = result * ((1 + x) % mod) % mod
        else:
            result *= (1 + x)
    return (result - 1) % mod if mod else result - 1

# --- Method 2: Newton's identities ---
def sum_esp_newton(xs):
    """Compute e_k via Newton's identities, then sum."""
    n = len(xs)
    # Power sums
    p = [0] * (n + 1)
    for j in range(1, n + 1):
        p[j] = sum(x**j for x in xs)
    # Newton recurrence
    e = [0] * (n + 1)
    e[0] = 1
    for k in range(1, n + 1):
        s = 0
        for j in range(1, k + 1):
            s += (-1)**(j - 1) * e[k - j] * p[j]
        e[k] = s // k
    return sum(e[1:])

# --- Method 3: Direct enumeration (brute force) ---
def sum_esp_brute(xs):
    """Enumerate all subsets."""
    from itertools import combinations
    n = len(xs)
    total = 0
    for k in range(1, n + 1):
        for subset in combinations(xs, k):
            prod = 1
            for v in subset:
                prod *= v
            total += prod
    return total

# --- Verification ---
test_cases = [
    [1],
    [1, 2],
    [1, 2, 3],
    [1, 2, 3, 4],
    [2, 3, 5],
    [1, 1, 1, 1, 1],
    [7, 11, 13],
]

for xs in test_cases:
    a = sum_esp_product(xs)
    b = sum_esp_newton(xs)
    c = sum_esp_brute(xs)
    assert a == b == c, f"Mismatch for {xs}: prod={a}, newton={b}, brute={c}"

print("All verification passed!")

# Compute for a larger input
xs_large = list(range(1, 1001))
answer = sum_esp_product(xs_large, MOD)
print(f"Answer: {answer}")
