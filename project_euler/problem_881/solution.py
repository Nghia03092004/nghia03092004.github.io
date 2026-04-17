"""
Problem 881: Divisor Graph Coloring
Chromatic number of divisibility graph G_n on {1,...,n}.
Result: chi(G_n) = floor(log2(n)) + 1 (by Dilworth's theorem).
"""

from math import log2, floor

def chromatic_number(n):
    """chi(G_n) = floor(log2(n)) + 1."""
    if n <= 0:
        return 0
    return floor(log2(n)) + 1

def longest_chain(n):
    """Find the longest divisibility chain in {1,...,n}."""
    chain = [1]
    while chain[-1] * 2 <= n:
        chain.append(chain[-1] * 2)
    return chain

def valid_coloring(n):
    """Assign colors: color(k) = floor(log2(k))."""
    return {k: floor(log2(k)) for k in range(1, n + 1)}

def verify_coloring(n, coloring):
    """Check that no two adjacent vertices share a color."""
    for i in range(1, n + 1):
        for j in range(2 * i, n + 1, i):
            if coloring[i] == coloring[j]:
                return False, (i, j)
    return True, None

def edge_count(n):
    """Count edges in G_n: sum_{k=1}^{n} (floor(n/k) - 1)."""
    total = 0
    for k in range(1, n + 1):
        total += n // k - 1
    return total

def edge_count_fast(n):
    """O(sqrt(n)) edge count using floor block decomposition."""
    # sum floor(n/k) for k=1..n, then subtract n
    total = 0
    k = 1
    while k <= n:
        v = n // k
        kp = n // v
        total += v * (kp - k + 1)
        k = kp + 1
    return total - n

# --- Verification ---
print("=== Chromatic Number Verification ===")
for n in [1, 2, 3, 4, 5, 8, 10, 16, 32, 64, 100, 1000]:
    chi = chromatic_number(n)
    chain = longest_chain(n)
    coloring = valid_coloring(n)
    ok, conflict = verify_coloring(n, coloring)
    print(f"  n={n:>4}: chi={chi}, chain={chain}, "
          f"coloring valid={'OK' if ok else f'FAIL at {conflict}'}")
    assert ok
    assert len(chain) == chi

print("\n=== Edge Counts ===")
for n in [5, 10, 20, 50, 100]:
    e1 = edge_count(n)
    e2 = edge_count_fast(n)
    print(f"  |E(G_{n})| = {e1} (brute), {e2} (fast), Match: {'OK' if e1 == e2 else 'FAIL'}")
    assert e1 == e2

print("\n=== Color Classes for n=16 ===")
coloring = valid_coloring(16)
classes = {}
for k, c in coloring.items():
    classes.setdefault(c, []).append(k)
for c in sorted(classes):
    print(f"  Color {c}: {classes[c]}")

answer = chromatic_number(10**18)
print(f"\nAnswer: chi(G_{{10^18}}) = {answer}")

# --- 4-Panel Visualization ---
