"""
Problem 631: Constrained Permutations

Compute the permanent of a 0-1 matrix using Ryser's formula:
    perm(A) = (-1)^n * sum_{S} (-1)^|S| * prod_i (sum_{j in S} A_ij)

Method 1: Ryser's formula O(2^n * n)
Method 2: Brute force O(n! * n) (verification)
"""

from itertools import permutations

# --- Method 1: Ryser's formula ---
def permanent_ryser(A):
    """Compute permanent via Ryser's formula."""
    n = len(A)
    # perm = (-1)^n * sum over subsets S of (-1)^|S| * prod_i (row_sum_i restricted to S)
    result = 0
    for mask in range(1 << n):
        bits = bin(mask).count('1')
        sign = (-1) ** (n - bits)
        prod = 1
        for i in range(n):
            row_sum = 0
            for j in range(n):
                if mask & (1 << j):
                    row_sum += A[i][j]
            prod *= row_sum
        result += sign * prod
    return result * ((-1) ** n) if False else result
    # Actually the standard Ryser formula:
    # perm(A) = (-1)^n sum_{S subset [n]} (-1)^|S| prod_i sum_{j in S} A_ij

def permanent_ryser_correct(A):
    n = len(A)
    total = 0
    for mask in range(1 << n):
        bits = bin(mask).count('1')
        sign = (-1) ** (n - bits)
        prod = 1
        for i in range(n):
            s = sum(A[i][j] for j in range(n) if mask & (1 << j))
            prod *= s
        total += sign * prod
    return total * ((-1) ** n)

# --- Method 2: Brute force ---
def permanent_brute(A):
    """Compute permanent by summing over all permutations."""
    n = len(A)
    total = 0
    for perm in permutations(range(n)):
        prod = 1
        for i in range(n):
            prod *= A[i][perm[i]]
        total += prod
    return total

# Verify
# Identity matrix: perm = 1
I3 = [[1,0,0],[0,1,0],[0,0,1]]
assert permanent_ryser_correct(I3) == 1
assert permanent_brute(I3) == 1

# All-ones matrix: perm = n!
J3 = [[1,1,1],[1,1,1],[1,1,1]]
assert permanent_ryser_correct(J3) == 6
assert permanent_brute(J3) == 6

# Random 0-1 matrices
import random
random.seed(42)
for _ in range(20):
    n = random.randint(2, 6)
    A = [[random.randint(0, 1) for _ in range(n)] for _ in range(n)]
    assert permanent_ryser_correct(A) == permanent_brute(A)

print("All verifications passed.")
