"""
Problem 913: Balanced Ternary Representation

Find the sum of all positive integers n <= 10^6 that can be represented
in balanced ternary using only the digits {-1, +1} (no zeros).

A balanced ternary representation uses digits {-1, 0, 1} with place values
3^0, 3^1, 3^2, ... . This problem restricts to representations where every
digit is either -1 or +1 (the "no-zero" or "non-adjacent" form).

Key results:
    - Each such number is a signed sum of distinct powers of 3
    - The set of representable numbers forms a self-similar fractal-like structure
    - With k digits, we get 2^k distinct sums covering a symmetric range
    - The number of representable values up to N grows roughly as N^(ln2/ln3)

Methods:
    1. Recursive set generation (BFS over digit positions)
    2. Brute-force check via ternary decomposition (verification)
    3. Counting function for density analysis
"""

def generate_nozero_balanced_ternary(N):
    """
    Generate all integers representable as sum of +/-3^k for distinct k,
    where every digit position used has digit +1 or -1 (no zeros).

    Returns the set of all such values (positive, negative, and zero
    are all included in the generation, but we only sum positives <= N).
    """
    # Start with the empty sum = 0, then for each power of 3,
    # every existing value branches into val+3^k and val-3^k.
    values = {0}
    current = {0}
    k = 0
    while 3**k <= 2 * N:
        new = set()
        for v in current:
            new.add(v + 3**k)
            new.add(v - 3**k)
        current = new
        values |= new
        k += 1
    return values

def solve(N=10**6):
    """Sum of all positive integers <= N representable in no-zero balanced ternary."""
    values = generate_nozero_balanced_ternary(N)
    return sum(v for v in values if 1 <= v <= N)

def is_nozero_balanced_ternary(n):
    """
    Check if n can be written as a sum of +/-3^k (each power used exactly once
    with +1 or -1). Greedy approach: look at balanced ternary digits.
    """
    if n == 0:
        return True
    # Convert to balanced ternary and check no zero digit
    val = abs(n)
    while val > 0:
        r = val % 3
        if r == 0:
            return False  # zero digit found
        if r == 1:
            val = val // 3
        elif r == 2:
            # digit is -1, so carry
            val = (val + 1) // 3
    return True

def solve_brute(N):
    """Check every integer 1..N individually."""
    return sum(k for k in range(1, N + 1) if is_nozero_balanced_ternary(k))

def count_representable(N):
    """Count of no-zero balanced ternary numbers in [1, N]."""
    values = generate_nozero_balanced_ternary(N)
    return sum(1 for v in values if 1 <= v <= N)

# Verification
for test_n in [20, 50, 100]:
    assert solve(test_n) == solve_brute(test_n), f"Mismatch at N={test_n}"

# First few representable positives: 1 (=3^0), 2 (=3^1-3^0), 3 (=3^1), 4 (=3^1+3^0), ...
assert is_nozero_balanced_ternary(1)   # +3^0
assert is_nozero_balanced_ternary(2)   # +3^1 - 3^0
assert not is_nozero_balanced_ternary(6)  # 6 = 2*3 -> has zero digit issue
assert is_nozero_balanced_ternary(4)   # +3^1 + 3^0

# Solve
answer = solve()
print(answer)
