"""
Problem 523: First Sort I
Expected number of moves in a specific insertion-sort variant.
E(n) = (n-1)(n+4)/12
"""

from fractions import Fraction
from itertools import permutations

def E_closed(n: int) -> Fraction:
    """Closed-form: E(n) = (n-1)(n+4)/12."""
    return Fraction((n - 1) * (n + 4), 12)

def simulate_first_sort(perm: list):
    """
    Simulate the first-sort algorithm on a permutation.
    Repeatedly find the first element > its successor, remove it,
    and reinsert it at the correct position.
    Return the number of moves.
    """
    arr = list(perm)
    moves = 0
    while True:
        found = -1
        for i in range(len(arr) - 1):
            if arr[i] > arr[i + 1]:
                found = i
                break
        if found == -1:
            break
        elem = arr.pop(found)
        inserted = False
        for j in range(len(arr)):
            if elem <= arr[j]:
                arr.insert(j, elem)
                inserted = True
                break
        if not inserted:
            arr.append(elem)
        moves += 1
    return moves

def E_exact(n: int) -> Fraction:
    """Exact E(n) via exhaustive enumeration (small n only)."""
    total = 0
    count = 0
    for p in permutations(range(1, n + 1)):
        total += simulate_first_sort(list(p))
        count += 1
    return Fraction(total, count)

# Verify closed form against exhaustive enumeration
print("Verification: E(n) = (n-1)(n+4)/12")
print(f"{'n':>4} {'Closed':>12} {'Exact':>12} {'Match':>6}")
for n in range(1, 9):
    cf = E_closed(n)
    ex = E_exact(n)
    match = "OK" if cf == ex else "FAIL"
    print(f"{n:4d} {str(cf):>12} {str(ex):>12} {match:>6}")

# Values for larger n
print("\nE(n) for larger n:")
for n in [10, 20, 50, 100, 1000]:
    print(f"  E({n}) = {E_closed(n)} = {float(E_closed(n)):.4f}")
