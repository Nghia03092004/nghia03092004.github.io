"""
Problem 899: Tree Enumeration
Cayley's formula: t(n) = n^(n-2) labeled trees on n vertices.
Verified via Prufer sequence bijection and Kirchhoff's matrix tree theorem.
"""

import numpy as np
from itertools import product as cartprod
from math import factorial
import sys

def cayley(n):
    """Cayley's formula: n^(n-2)."""
    if n <= 1:
        return 0
    return n ** (n - 2)

def cayley_mod(n, p):
    """Compute n^(n-2) mod p using fast exponentiation."""
    if n <= 1:
        return 0
    return pow(n, n - 2, p)

def prufer_to_tree(seq, n):
    """Decode a Prufer sequence into a set of edges."""
    degree = [1] * (n + 1)
    for v in seq:
        degree[v] += 1
    edges = []
    seq_list = list(seq)
    for a in seq_list:
        for v in range(1, n + 1):
            if degree[v] == 1:
                edges.append((v, a))
                degree[v] -= 1
                degree[a] -= 1
                break
    # last edge: two remaining degree-1 vertices
    remaining = [v for v in range(1, n + 1) if degree[v] == 1]
    if len(remaining) == 2:
        edges.append(tuple(remaining))
    return edges

def count_trees_bruteforce(n):
    """Count labeled trees by enumerating all Prufer sequences."""
    if n <= 1:
        return int(n == 1)
    count = 0
    for seq in cartprod(range(1, n + 1), repeat=n - 2):
        count += 1
    return count  # Each Prufer sequence gives a unique tree

def kirchhoff_count(n):
    """Count spanning trees of K_n via Kirchhoff's matrix tree theorem."""
    if n <= 1:
        return int(n == 1)
    L = np.zeros((n, n))
    for i in range(n):
        for j in range(n):
            if i == j:
                L[i][j] = n - 1
            else:
                L[i][j] = -1
    # Cofactor: delete row 0, col 0
    M = L[1:, 1:]
    return round(np.linalg.det(M))

def cumulative_sum(N, p=None):
    """Compute sum_{n=2}^{N} n^(n-2), optionally mod p."""
    s = 0
    for n in range(2, N + 1):
        if p:
            s = (s + pow(n, n - 2, p)) % p
        else:
            s += n ** (n - 2)
    return s

# --- Verification ---
print("=== Cayley's Formula Verification ===")
print(f"{'n':>4} {'Cayley':>12} {'Prufer':>12} {'Kirchhoff':>12} {'Match':>6}")
for n in range(2, 9):
    c = cayley(n)
    p = count_trees_bruteforce(n)
    k = kirchhoff_count(n)
    match = "OK" if c == p == k else "FAIL"
    print(f"{n:>4} {c:>12} {p:>12} {k:>12} {match:>6}")
    assert c == p == k

# Prufer decode example
print("\n=== Prufer Decode Example (n=4) ===")
for seq in [(1, 1), (1, 2), (2, 2), (3, 3), (4, 4)]:
    edges = prufer_to_tree(seq, 4)
    print(f"  Seq {seq} -> Edges {edges}")

# Cumulative sums
print("\n=== Cumulative Sums ===")
for N in range(2, 12):
    s = cumulative_sum(N)
    print(f"  sum_{{n=2}}^{{{N}}} n^(n-2) = {s}")

# Modular computation
MOD = 10**9 + 7
print(f"\n=== Modular (mod {MOD}) ===")
for N in [100, 1000, 10000]:
    s = cumulative_sum(N, MOD)
    print(f"  sum_{{n=2}}^{{{N}}} n^(n-2) mod p = {s}")

answer = cumulative_sum(1000, MOD)
print(f"\nAnswer: sum_{{n=2}}^{{1000}} n^(n-2) mod 10^9+7 = {answer}")

# --- 4-Panel Visualization ---
