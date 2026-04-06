"""
Problem 622: Riffle Shuffles

A perfect (out-)riffle shuffle on 2n cards has order ord_{2n+1}(2).
Find sum of deck sizes 2n where the shuffle order is exactly 60.

Key identity: ord_m(2) = 60 iff m | 2^60 - 1 and 2^{60/p} != 1 mod m
for each prime p | 60 (i.e., p in {2, 3, 5}).

2^60 - 1 = 3^2 * 5^2 * 7 * 11 * 13 * 31 * 41 * 61 * 151 * 331 * 1321

Method 1: Enumerate all divisors, check order (primary)
Method 2: Compute ord_m(2) directly for small m (verification)
"""

from itertools import product

# --- Multiplicative order ---
def mult_order(a: int, m: int):
    """Compute multiplicative order of a modulo m."""
    if m == 1:
        return 1
    order, cur = 1, a % m
    while cur != 1:
        cur = cur * a % m
        order += 1
        if order > m:
            return -1  # a and m not coprime
    return order

def check_order_exact(a: int, k: int, m: int, prime_divs: list) -> bool:
    """Check if ord_m(a) == k using the prime-divisor criterion.

    ord_m(a) = k iff a^k = 1 mod m AND a^{k/p} != 1 mod m for all primes p | k.
    """
    if pow(a, k, m) != 1:
        return False
    for p in prime_divs:
        if pow(a, k // p, m) == 1:
            return False
    return True

# --- Method 1: Enumerate divisors of 2^60 - 1 ---
def solve_divisor_enumeration():
    """Enumerate all divisors of 2^60 - 1 and find those with ord = 60."""
    # Prime factorization of 2^60 - 1
    factors = [(3, 2), (5, 2), (7, 1), (11, 1), (13, 1),
               (31, 1), (41, 1), (61, 1), (151, 1), (331, 1), (1321, 1)]

    # Verify factorization
    check = 1
    for p, e in factors:
        check *= p ** e
    assert check == 2**60 - 1, f"Factorization error: {check} != {2**60 - 1}"

    total = 0
    valid_m = []
    for exps in product(*[range(e + 1) for _, e in factors]):
        d = 1
        for i, (p, _) in enumerate(factors):
            d *= p ** exps[i]
        if d <= 1:
            continue
        if check_order_exact(2, 60, d, [2, 3, 5]):
            total += d - 1
            valid_m.append(d)

    return total, sorted(valid_m)

# --- Method 2: Brute force for small deck sizes ---
def solve_brute(target_order: int, max_deck: int = 10000):
    """Find deck sizes 2n <= max_deck with shuffle order = target_order."""
    results = []
    for deck_size in range(2, max_deck + 1, 2):
        m = deck_size + 1
        if mult_order(2, m) == target_order:
            results.append(deck_size)
    return results

# Compute and verify
answer, valid_m_list = solve_divisor_enumeration()

# Cross-check: brute force for small values
brute_results = solve_brute(60, 10000)
enum_small = [m - 1 for m in valid_m_list if m - 1 <= 10000]
assert sorted(brute_results) == sorted(enum_small), \
    f"Mismatch:\nBrute: {brute_results}\nEnum:  {enum_small}"

# Verify specific known values
assert mult_order(2, 3) == 2   # deck 2
assert mult_order(2, 5) == 4   # deck 4
assert mult_order(2, 7) == 3   # deck 6
assert mult_order(2, 9) == 6   # deck 8
assert mult_order(2, 15) == 4  # deck 14: lcm(ord_3, ord_5) = lcm(2,4) = 4

# Verify 2^60 - 1 factorization
assert 2**60 - 1 == 3**2 * 5**2 * 7 * 11 * 13 * 31 * 41 * 61 * 151 * 331 * 1321

print(f"Number of valid m values: {len(valid_m_list)}")
print(f"Smallest valid deck sizes: {[m-1 for m in valid_m_list[:10]]}")
print(f"Answer: {answer}")
assert answer == 3010983666182123972
