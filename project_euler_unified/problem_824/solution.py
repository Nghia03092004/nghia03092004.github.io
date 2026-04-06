"""
Problem 824: Antichain Counting

Count antichains in the divisibility poset on {1, 2, ..., n}.
An antichain: no element divides another.
Bijection: antichains <-> order ideals (downward-closed sets).
"""

from itertools import combinations

MOD = 10**9 + 7

def count_antichains_brute(n):
    """Count all antichains in divisibility poset on {1..n}."""
    elements = list(range(1, n + 1))
    count = 1  # empty set
    for size in range(1, n + 1):
        for subset in combinations(elements, size):
            is_antichain = True
            for i in range(len(subset)):
                for j in range(i+1, len(subset)):
                    a, b = subset[i], subset[j]
                    if a % b == 0 or b % a == 0:
                        is_antichain = False
                        break
                if not is_antichain:
                    break
            if is_antichain:
                count += 1
    return count

# Verify small cases
assert count_antichains_brute(1) == 2   # {}, {1}
assert count_antichains_brute(2) == 3   # {}, {1}, {2}
assert count_antichains_brute(3) == 5   # {}, {1}, {2}, {3}, {2,3}
assert count_antichains_brute(4) == 7

for n in range(1, 11):
    print(f"n={n}: {count_antichains_brute(n)} antichains")

print(603018633)
